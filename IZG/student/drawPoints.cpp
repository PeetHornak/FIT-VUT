#include <student/gpu.h>

#include<iostream>
#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<math.h>

bool isPointInsideViewFrustum(Vec4 const*const p){
  // -Aw <= Ai <= +Aw
  for(int i=0;i<3;++i){
    if(p->data[i] <= -p->data[3])return false;
    if(p->data[i] >= +p->data[3])return false;
  }
  return true;
}

void doPerspectiveDivision(Vec4*const ndc,Vec4 const*const p){
  for(int a=0;a<3;++a)
    ndc->data[a] = p->data[a]/p->data[3];
  ndc->data[3] = p->data[3];
}

Vec4 computeFragPosition(Vec4 const&p,uint32_t width,uint32_t height){
  Vec4 res;
  res.data[0] = (p.data[0]*.5f+.5f)*width;
  res.data[1] = (p.data[1]*.5f+.5f)*height;
  res.data[2] = p.data[2];
  res.data[3] = p.data[3];
  return res;
}

void copyVertexAttribute(GPU const*const gpu,GPUAttribute*const att,GPUVertexPullerHead const*const head,uint64_t vertexId){
  if(!head->enabled)return;
  GPUBuffer const*const buf = gpu_getBuffer(gpu,head->bufferId);
  uint8_t const*ptr = (uint8_t*)buf->data;
  uint32_t const offset = (uint32_t)head->offset;
  uint32_t const stride = (uint32_t)head->stride;
  uint32_t const size   = (uint32_t)head->type;
  memcpy(att->data,ptr+offset + vertexId*stride,size);
}

void lobotomized_vertexPuller(GPUInVertex*const inVertex,GPUVertexPuller const*const vao,GPU const*const gpu,uint32_t vertexSHaderInvocation){
  uint32_t vertexId = vertexSHaderInvocation;
  inVertex->gl_VertexID = vertexId;

  copyVertexAttribute(gpu,inVertex->attributes,vao->heads,vertexId);
  copyVertexAttribute(gpu,inVertex->attributes+1,vao->heads+1,vertexId);
}

void lobotomized_pointRasterization(GPUInFragment*const inFragment,Vec4 ndc,GPU*const gpu,GPUOutVertex const*const outVertex){
  Vec4 coord = computeFragPosition(ndc,gpu->framebuffer.width,gpu->framebuffer.height);
  inFragment->gl_FragCoord = coord;
  memcpy(inFragment->attributes[0].data,outVertex->attributes[0].data,sizeof(Vec4));
}

void lobotomized_perFragmentOperation(GPUOutFragment const*const outFragment,GPU*const gpu,Vec4 ndc){
  Vec4 coord = computeFragPosition(ndc,gpu->framebuffer.width,gpu->framebuffer.height);
  GPUFramebuffer*const frame = &gpu->framebuffer;
  if(coord.data[0] < 0 || coord.data[0] >= frame->width)return;
  if(coord.data[1] < 0 || coord.data[1] >= frame->height)return;
  if(isnan(coord.data[0]))return;
  if(isnan(coord.data[1]))return;
  uint32_t const pixCoord = frame->width*(int)coord.data[1]+(int)coord.data[0];

  frame->color[pixCoord] = outFragment->gl_FragColor;
}

/**
 * @brief This function draws points
 * 
 * @param gpu gpu
 * @param nofVertices number of vertices
 */
void gpu_drawPoints(GPU*const gpu,uint32_t nofVertices){
  GPUProgram      const* prg = gpu_getActiveProgram(gpu);
  GPUVertexPuller const* vao = gpu_getActivePuller (gpu);

  GPUVertexShaderData   vd;
  GPUFragmentShaderData fd;

  vd.uniforms = &prg->uniforms;

  for(uint32_t v=0;v<nofVertices;++v){

    lobotomized_vertexPuller(&vd.inVertex,vao,gpu,v);

    prg->vertexShader(&vd);

    Vec4 pos;
    copy_Vec4(&pos,&vd.outVertex.gl_Position);
    if(!isPointInsideViewFrustum(&pos))continue;

    Vec4 ndc;
    doPerspectiveDivision(&ndc,&pos);

    lobotomized_pointRasterization(&fd.inFragment,ndc,gpu,&vd.outVertex);

    prg->fragmentShader(&fd);

    lobotomized_perFragmentOperation(&fd.outFragment,gpu,ndc);
  }
}
