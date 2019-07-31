#include <student/gpu.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

/**
 * Parts of this code were inspired by code from drawPoints.cpp
 **/

void copyVertexAttributes(GPU const*const gpu,GPUAttribute*const att,GPUVertexPullerHead const*const head,uint64_t vertexId){
    if(!head->enabled)return;
    GPUBuffer const*const buf = gpu_getBuffer(gpu,head->bufferId);
    uint8_t const*ptr = (uint8_t*)buf->data;
    uint32_t const offset = (uint32_t)head->offset;
    uint32_t const stride = (uint32_t)head->stride;
    uint32_t const size   = (uint32_t)head->type;
    memcpy(att->data,ptr+offset + vertexId*stride,size);
}

void vertexPuller(GPUInVertex*const inVertex,GPUVertexPuller const*const puller,GPU const*const gpu,uint32_t vertexShaderInvocation){
    uint32_t bufferId = puller->indices.bufferId;
    uint32_t vertexId;

    int isBuffer = gpu_isBuffer(gpu, bufferId);
    if (isBuffer) {
        GPUBuffer const*const buf = gpu_getBuffer(gpu, bufferId);
        int indexType = puller->indices.type;
        if (indexType == 1){
            uint8_t *data = (uint8_t*)buf->data;
            vertexId = data[vertexShaderInvocation];
        } else if (indexType == 2){
            uint16_t *data = (uint16_t*)buf->data;
            vertexId = data[vertexShaderInvocation];
        } else {
            uint32_t *data = (uint32_t*)buf->data;
            vertexId = data[vertexShaderInvocation];
        }
    }
    else
        vertexId = vertexShaderInvocation;

    inVertex->gl_VertexID = vertexId;

    for (int i = 0; i < MAX_ATTRIBUTES; i++) {
        copyVertexAttributes(gpu, inVertex->attributes + i, puller->heads + i, vertexId);
    }
}


void perspectiveDivision(Vec4*const ndc,Vec4 const*const p){
    for(int a=0;a<3;++a)
        ndc->data[a] = p->data[a]/p->data[3];
    ndc->data[3] = p->data[3];
}

Vec4 fragPosition(Vec4 const&p,uint32_t width,uint32_t height){
    Vec4 res;
    res.data[0] = (p.data[0]*.5f+.5f)*width;
    res.data[1] = (p.data[1]*.5f+.5f)*height;
    res.data[2] = p.data[2];
    res.data[3] = p.data[3];
    return res;
}

void perFragmentOperation(GPUOutFragment const*const outFragment, GPUInFragment const*const inFragment,GPU*const gpu,Vec4 coord){
    GPUFramebuffer*const frame = &gpu->framebuffer;
    if(coord.data[0] < 0 || coord.data[0] >= frame->width)return;
    if(coord.data[1] < 0 || coord.data[1] >= frame->height)return;
    if(isnan(coord.data[0]))return;
    if(isnan(coord.data[1]))return;
    uint32_t const pixCoord = frame->width*(int)coord.data[1]+(int)coord.data[0];

    if (frame->depth[pixCoord] > inFragment->gl_FragCoord.data[2]){
        frame->color[pixCoord] = outFragment->gl_FragColor;
        frame->depth[pixCoord] = inFragment->gl_FragCoord.data[2];
    }

}

void rasterization(GPUFragmentShaderData* fragmentData, Vec4 ndc[],GPU*const gpu, GPUVertexShaderData vertexData[]){
    Vec4 coords[3];
    for (int i = 0; i < 3; i++){
        coords[i] = fragPosition(ndc[i],gpu->framebuffer.width,gpu->framebuffer.height);
    }

    int minX, maxX = coords[0].data[0];
    int minY, maxY = coords[0].data[1];
    for (int i = 1; i < 3; i++){
        if (coords[i].data[0] > maxX)
            maxX = coords[i].data[0];
        if (coords[i].data[0] < minX)
            minX = coords[i].data[0];
        if (coords[i].data[1] > maxY)
            maxY = coords[i].data[1];
        if (coords[i].data[1] < minY)
            minY = coords[i].data[1];
    }

    if (minX < 0.f)
        minX = 0.f;
    if (minY < 0.f)
        minY = 0.f;
    if (maxX > gpu->framebuffer.width - 1)
        maxX = gpu->framebuffer.width - 1;
    if (maxY > gpu->framebuffer.height - 1)
        maxY = gpu->framebuffer.height - 1;

    int deltaX1 = coords[1].data[0] - coords[0].data[0];
    int deltaX2 = coords[2].data[0] - coords[1].data[0];
    int deltaX3 = coords[0].data[0] - coords[2].data[0];

    int deltaY1 = coords[1].data[1] - coords[0].data[1];
    int deltaY2 = coords[2].data[1] - coords[1].data[1];
    int deltaY3 = coords[0].data[1] - coords[2].data[1];

    int edgeF1 = (minY - coords[0].data[1]) * deltaX1 - (minX - coords[0].data[0]) * deltaY1;
    int edgeF2 = (minY - coords[1].data[1]) * deltaX2 - (minX - coords[1].data[0]) * deltaY2;
    int edgeF3 = (minY - coords[2].data[1]) * deltaX3 - (minX - coords[2].data[0]) * deltaY3;

    for (int ny = minY; ny <= maxY; ny++){
        bool even = (ny - minY) % 2 == 0;
        float y = ny + 0.5f;

        for (int nx = ((even) ? minX : maxX); (even) ? (nx <= maxX) : (nx >= minX); nx += (even) ? 1 : -1){
            float x = nx + 0.5f;
            if (edgeF1 >= 0 && edgeF2 >= 0 && edgeF3 >= 0){
                Vec4 currentCoord;
                currentCoord.data[0] = x;
                currentCoord.data[1] = y;


                float v0x = abs(coords[1].data[0] - coords[0].data[0]);
                float v0y = abs(coords[1].data[1] - coords[0].data[1]);
                float v1x = abs(coords[2].data[0] - coords[0].data[0]);
                float v1y = abs(coords[2].data[1] - coords[0].data[1]);
                float v2x = abs(coords[1].data[0] - coords[2].data[0]);
                float v2y = abs(coords[1].data[1] - coords[2].data[1]);
                float area = sqrt(v0x * v0x + v0y * v0y) * sqrt(v1x * v1y + v1x * v1y) / 2.f;
                float w0x = abs(currentCoord.data[0] - coords[0].data[0]);
                float w0y = abs(currentCoord.data[1] - coords[0].data[1]);
                float w1x = abs(currentCoord.data[0] - coords[1].data[0]);
                float w1y = abs(currentCoord.data[1] - coords[1].data[1]);
                float w2x = abs(currentCoord.data[0] - coords[2].data[0]);
                float w2y = abs(currentCoord.data[1] - coords[2].data[1]);
                float v = (sqrt(w1x * w1x + w1y * w1y) * sqrt(w2x * w2y + w2x * w2y) / 2.f) / area;
                float w = (sqrt(w2x * w2x + w2y * w2y) * sqrt(w0x * w0y + w0x * w0y) / 2.f) / area;
                float u = (sqrt(w0x * w0x + w0y * w0y) * sqrt(w1x * w1y + w1x * w1y) / 2.f) / area;


                float lambda0 = v / coords[0].data[3];
                float lambda1 = w / coords[1].data[3];
                float lambda2 = u / coords[2].data[3];
                GPUProgram const* prg = gpu_getActiveProgram(gpu);
                for (int i = 0; i < MAX_ATTRIBUTES; i++){
                    if (prg->vs2fsType[i] == ATTRIBUTE_EMPTY)
                        continue;
                    else if(prg->vs2fsType[i] == ATTRIBUTE_FLOAT){
                        float vertexToVec[3];
                        for (int j = 0; j < 3; j++){
                            vertexToVec[j] = *vertexData[j].outVertex.attributes[i].data;
                        }
                        for (int j = 0; j < 1; j++){
                            float uk = (vertexToVec[0] * lambda0 + vertexToVec[1] * lambda1 + vertexToVec[2] * lambda2) / (lambda0 + lambda1 + lambda2);
                            memcpy(&fragmentData->inFragment.attributes[i].data + sizeof(float) * j, &uk, sizeof(float));
                        }
                    }
                    else if(prg->vs2fsType[i] == ATTRIBUTE_VEC2){
                        Vec2 vertexToVec[3];
                        for (int j = 0; j < 3; j++){
                            memcpy(&vertexToVec[j].data, vertexData[j].outVertex.attributes[i].data, sizeof(Vec2));
                        }
                        for (int j = 0; j < 2; j++){
                            float uk = (vertexToVec[0].data[j] * lambda0 + vertexToVec[1].data[j] * lambda1 + vertexToVec[2].data[j] * lambda2) / (lambda0 + lambda1 + lambda2);
                            memcpy(&fragmentData->inFragment.attributes[i].data + sizeof(float) * j, &uk, sizeof(float));
                        }
                    }
                    else if(prg->vs2fsType[i] == ATTRIBUTE_VEC3){
                        Vec3 vertexToVec[3];
                        for (int j = 0; j < 3; j++){
                            memcpy(&vertexToVec[j].data, vertexData[j].outVertex.attributes[i].data, sizeof(Vec3));
                        }
                        for (int j = 0; j < 3; j++){
                            float uk = (vertexToVec[0].data[j] * lambda0 + vertexToVec[1].data[j] * lambda1 + vertexToVec[2].data[j] * lambda2) / (lambda0 + lambda1 + lambda2);
                            memcpy(&fragmentData->inFragment.attributes[i].data + sizeof(float) * j, &uk, sizeof(float));

                        }

                    }
                    else if(prg->vs2fsType[i] == ATTRIBUTE_VEC4){
                        Vec4 vertexToVec[3];
                        for (int j = 0; j < 3; j++){
                            memcpy(&vertexToVec[j].data, vertexData[j].outVertex.attributes[i].data, sizeof(Vec4));
                        }
                        for (int j = 0; j < 4; j++){
                            float uk = (vertexToVec[0].data[j] * lambda0 + vertexToVec[1].data[j] * lambda1 + vertexToVec[2].data[j] * lambda2) / (lambda0 + lambda1 + lambda2);
                            memcpy(&fragmentData->inFragment.attributes[i].data + sizeof(float) * j, &uk, sizeof(float));
                        }
                    }
                }

                currentCoord.data[2] = (coords[0].data[2] * lambda0 + coords[1].data[2] * lambda1 + coords[2].data[2] * lambda2) / (lambda0 + lambda1 + lambda2);
                currentCoord.data[3] = 1;

                fragmentData->inFragment.gl_FragCoord = currentCoord;
                fragmentData->uniforms = &prg->uniforms;
                prg->fragmentShader(fragmentData);
                perFragmentOperation(&fragmentData->outFragment, &fragmentData->inFragment, gpu, currentCoord);
            }

            if (!((even && x == maxX) || (!even && x == minX))){
                edgeF1 += even ? -deltaY1 : deltaY1;
                edgeF2 += even ? -deltaY2 : deltaY2;
                edgeF3 += even ? -deltaY3 : deltaY3;
            }
        }
        edgeF1 += deltaX1;
        edgeF2 += deltaX2;
        edgeF3 += deltaX3;
    }

}

/// \addtogroup gpu_side Implementace vykreslovacího řetězce - vykreslování trojúhelníků
/// @{

/**
 * @brief This function should draw triangles
 *
 * @param gpu gpu 
 * @param nofVertices number of vertices
 */
void gpu_drawTriangles(GPU *const gpu, uint32_t nofVertices)
{

    GPUProgram      const* program = gpu_getActiveProgram(gpu);
    GPUVertexPuller const* puller = gpu_getActivePuller (gpu);

    GPUVertexShaderData   vertex_data;
    GPUFragmentShaderData fragment_data;

    vertex_data.uniforms = &program->uniforms;
    Vec4 arr[3];
    GPUVertexShaderData vArr[3];
    for(uint32_t v=1;v<=nofVertices;++v) {

        vertexPuller(&vertex_data.inVertex, puller, gpu, v-1);
        program->vertexShader(&vertex_data);

        Vec4 pos;
        copy_Vec4(&pos, &vertex_data.outVertex.gl_Position);

        Vec4 ndc;
        perspectiveDivision(&ndc, &pos);

        int mod3 = v % 3;
        arr[mod3] = ndc;
        vArr[mod3] = vertex_data;
        if (mod3 == 0){
            rasterization(&fragment_data, arr, gpu, vArr);
        }
    }
}
