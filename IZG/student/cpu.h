/*!
 * @file
 * @brief This file contains declarations of API for controling of software GPU.
 * API contains: 
 * functions for GPU buffer management
 * functions for Vertex Pullers management
 * functions for Shader Programs management
 * functions for drawing, clearing framebuffer and setting GPU state.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#pragma once

#include <SDL.h>
#include <student/gpu.h>


#ifdef __cplusplus
extern "C" {
#endif


void cpu_initGPU(GPU *const gpu,uint32_t width,uint32_t height);
void cpu_freeGPU(GPU *const gpu);
void cpu_enableDebug(GPU* const gpu);
void cpu_disableDebug(GPU* const gpu);

BufferID cpu_createBuffer(GPU*const gpu);
void     cpu_deleteBuffer(GPU*const gpu,BufferID id);
void     cpu_bufferData(GPU*const gpu,BufferID id,uint64_t size,void*data);
int      cpu_isBuffer(GPU const*const gpu,BufferID id);

VertexPullerID cpu_createVertexPuller     (GPU*const gpu);
void           cpu_deleteVertexPuller     (GPU*const gpu,VertexPullerID id);
void           cpu_setVertexPuller        (GPU*const gpu,VertexPullerID pullerId,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID bufferId);
void           cpu_setVertexPullerIndexing(GPU*const gpu,VertexPullerID pullerId,IndexType type,BufferID buffer);
void           cpu_enableVertexPullerHead (GPU*const gpu,VertexPullerID pullerId,uint32_t head);
void           cpu_disableVertexPullerHead(GPU*const gpu,VertexPullerID pullerId,uint32_t head);
void           cpu_bindVertexPuller       (GPU*const gpu,VertexPullerID id);
void           cpu_unbindVertexPuller     (GPU*const gpu);
int            cpu_isVertexPuller(GPU const*const gpu,VertexPullerID id);

ProgramID cpu_createProgram         (GPU*const gpu);
void      cpu_attachShaders         (GPU*const gpu,ProgramID programId,VertexShader vs,FragmentShader fs);
void      cpu_setVS2FSType          (GPU*const gpu,ProgramID programId,uint32_t attrib,AttributeType type);
void      cpu_deleteProgram         (GPU*const gpu,ProgramID programId);
void      cpu_useProgram            (GPU*const gpu,ProgramID programId);
int       cpu_isProgram             (GPU const*const gpu,ProgramID id);
void      cpu_programUniform1f      (GPU*const gpu,ProgramID programId,uint32_t uniformId,float const d);
void      cpu_programUniform2f      (GPU*const gpu,ProgramID programId,uint32_t uniformId,Vec2 const d);
void      cpu_programUniform3f      (GPU*const gpu,ProgramID programId,uint32_t uniformId,Vec3 const d);
void      cpu_programUniform4f      (GPU*const gpu,ProgramID programId,uint32_t uniformId,Vec4 const d);
void      cpu_programUniformMatrix4f(GPU*const gpu,ProgramID programId,uint32_t uniformId,Mat4 const d);

void cpu_clearColor(GPU* const gpu,Vec4 color);
void cpu_clearDepth(GPU* const gpu,float depth);
void cpu_drawPoints(GPU* const gpu,uint32_t nofVertices);
void cpu_drawTriangles(GPU* const gpu,uint32_t nofVertices);
void cpu_clear(GPU* const gpu);
void cpu_finish(GPU* const gpu);
void cpu_swapBuffers(SDL_Surface *const surface, GPU *const gpu);

#ifdef __cplusplus
}
#endif
