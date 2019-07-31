/*!
 * @file
 * @brief This file contains implementation of API for controlling software GPU.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#include <SDL_pixels.h>
#include <assert.h>
#include <student/cpu.h>

/**
 * @brief This function inits the GPU.
 *
 * @param gpu gpu
 * @param width width of the window
 * @param height height of the window
 */
void cpu_initGPU(GPU* const gpu, uint32_t width, uint32_t height)
{
  assert(gpu != NULL);
  gpu_initBuffers(&gpu->buffers);
  gpu_initVertexPullers(&gpu->pullers);
  gpu_initPrograms(&gpu->programs);
  gpu->activeVertexPuller = EMPTY_ID;
  gpu->activeProgram      = EMPTY_ID;
  gpu_allocateFramebuffer(&gpu->framebuffer, width, height);
  queue_init(&gpu->commandQueue, sizeof(GPUCommand));
  zero_Vec4(&gpu->clearColor);
  gpu->clearDepth = 1.f;
  gpu->debug = 0;
}

/**
 * @brief This function frees the gpu
 *
 * @param gpu gpu
 */
void cpu_freeGPU(GPU *const gpu){
  assert(gpu != NULL);
  gpu_freeFramebuffer(&gpu->framebuffer);
  gpu_freeBuffers(&gpu->buffers);
  gpu_freePrograms(&gpu->programs);
  gpu_freeVertexPullers(&gpu->pullers);
}

/**
 * @brief This function enables debug outputs
 *
 * @param gpu gpu
 */
void cpu_enableDebug(GPU* const gpu){
  assert(gpu != NULL);
  gpu->debug = 1;
}
/**
 * @brief This function disables debug outputs
 *
 * @param gpu gpu
 */
void cpu_disableDebug(GPU* const gpu){
  assert(gpu != NULL);
  gpu->debug = 0;
}

/**
 * @brief This function returns free id if it exists
 *
 * @param freeIds free id stack
 *
 * @return free id or EMPTY_IDD
 */
uint32_t ifFreeIdExistReturn(Stack *const freeIds){
  if (stack_empty(freeIds)) return EMPTY_ID;
  uint32_t id;
  stack_pop(freeIds, &id);
  return id;
}

/**
 * @brief This function adds element to a vector and returns its index
 *
 * @param v
 * @param data
 *
 * @return index to the element
 */
uint32_t addToVectorAndReturnIndex(Vector *const v,void*data){
  vector_pushback(v, data);
  return vector_size(v) - 1;
}

/**
 * @brief This function reserves buffer id
 *
 * @param gpu gpu
 *
 * @return id of reserved buffer
 */
BufferID cpu_createBuffer(GPU* const gpu)
{
  assert(gpu != NULL);

  BufferID id = ifFreeIdExistReturn(&gpu->buffers.freeIds);
  if(id != EMPTY_ID)return id;

  GPUBuffer buffer;
  gpu_initBuffer(&buffer);
  return addToVectorAndReturnIndex(&gpu->buffers.buffers, &buffer);
}

/**
 * @brief This function deletes buffer
 *
 * @param gpu gpu
 * @param id buffer id
 */
void cpu_deleteBuffer(GPU* const gpu, BufferID id)
{
  assert(gpu != NULL);
  assert(id < vector_size(&gpu->buffers.buffers));
  GPUBuffer* buffer = (GPUBuffer*)vector_at(&gpu->buffers.buffers, id);
  gpu_freeBuffer(buffer);
  stack_push(&gpu->buffers.freeIds, &id);
}

/**
 * @brief This function check if id points to valid buffer
 *
 * @param gpu gpu
 * @param id buffer id
 *
 * @return 1 if id points to buffer, 0 otherwise
 */
int cpu_isBuffer(GPU const*const gpu,BufferID id){
  assert(gpu != NULL);
  return gpu_isBuffer(gpu,id);
}

/**
 * @brief This function activates selected program
 *
 * @param gpu gpu
 * @param programId program id
 */
void cpu_useProgram(GPU*const gpu,ProgramID programId){
  assert(gpu != NULL);
  gpu_pushCommand1(gpu,COMMAND_USE_PROGRAM,programId);
}

/**
 * @brief This function allocates data on gpu and upload data (if specified)
 *
 * @param gpu gpu
 * @param id buffer id
 * @param size size of buffer in bytes
 * @param data NULL (allocation only) or pointer to cpu data
 */
void cpu_bufferData(GPU* const gpu, BufferID id, uint64_t size, void* data)
{
  assert(gpu != NULL);
  assert(id < vector_size(&gpu->buffers.buffers));
  GPUBuffer* buffer = (GPUBuffer*)vector_at(&gpu->buffers.buffers, id);
  gpu_bufferData(buffer, size, data);
}

/**
 * @brief This function reserves new vertex puller.
 *
 * @param gpu gpu
 *
 * @return id of new vertex puller
 */
VertexPullerID cpu_createVertexPuller(GPU* const gpu)
{
  assert(gpu != NULL);

  VertexPullerID id = ifFreeIdExistReturn(&gpu->pullers.freeIds);
  if(id != EMPTY_ID)return id;

  GPUVertexPuller newElement;
  gpu_initVertexPuller(&newElement);

  return addToVectorAndReturnIndex(&gpu->pullers.pullers, &newElement);
}

/**
 * @brief This function deletes vertex puller.
 *
 * @param gpu gpu
 * @param id id of vertex puller
 */
void cpu_deleteVertexPuller(GPU* const gpu, VertexPullerID id)
{
  assert(gpu != NULL);
  assert(id < vector_size(&gpu->pullers.pullers));
  GPUVertexPuller* puller =
      (GPUVertexPuller*)vector_at(&gpu->pullers.pullers, id);
  gpu_initVertexPuller(puller);
  stack_push(&gpu->pullers.freeIds, &id);
}

/**
 * @brief This function sets vertex puller head
 *
 * @param gpu gpu
 * @param pullerId puller id
 * @param head id reading head
 * @param type type of vertex attribute
 * @param stride reading stride
 * @param offset reading offset
 * @param bufferId buffer id
 */
void cpu_setVertexPuller(GPU* const     gpu,
                         VertexPullerID pullerId,
                         uint32_t       head,
                         AttributeType  type,
                         uint64_t       stride,
                         uint64_t       offset,
                         BufferID       bufferId)
{
  assert(gpu != NULL);
  assert(gpu_isPuller(gpu,pullerId));
  assert(head < MAX_ATTRIBUTES);
  GPUVertexPuller* puller = 
      (GPUVertexPuller*)vector_at(&gpu->pullers.pullers, pullerId);
  puller->heads[head].stride   = stride;
  puller->heads[head].offset   = offset;
  puller->heads[head].bufferId = bufferId;
  puller->heads[head].type     = type;
}

/**
 * @brief This function sets vertex puller indexing
 *
 * @param gpu gpu 
 * @param pullerId puller id
 * @param type type of indices
 * @param buffer buffer that contains indices
 */
void cpu_setVertexPullerIndexing(GPU*const gpu,VertexPullerID pullerId,IndexType type,BufferID buffer){
  assert(gpu != NULL);
  assert(gpu_isPuller(gpu,pullerId));
  GPUVertexPuller* puller = 
      (GPUVertexPuller*)vector_at(&gpu->pullers.pullers, pullerId);
  puller->indices.bufferId = buffer;
  puller->indices.type     = type;
}

/**
 * @brief This function enables vertex puller head.
 *
 * @param gpu gpu
 * @param pullerId id of vertex puller
 * @param head id of reading head
 */
void cpu_enableVertexPullerHead(GPU* const     gpu,
                                VertexPullerID pullerId,
                                uint32_t       head)
{
  assert(gpu != NULL);
  assert(pullerId < vector_size(&gpu->pullers.pullers));
  assert(head < MAX_ATTRIBUTES);
  GPUVertexPuller* puller =
      (GPUVertexPuller*)vector_at(&gpu->pullers.pullers, pullerId);
  puller->heads[head].enabled = 1;
}

/**
 * @brief This function disables vertex puller head.
 *
 * @param gpu gpu
 * @param pullerId id of vertex puller
 * @param head id of reading head
 */
void cpu_disableVertexPullerHead(GPU* const     gpu,
                                 VertexPullerID pullerId,
                                 uint32_t       head)
{
  assert(gpu != NULL);
  assert(pullerId < vector_size(&gpu->pullers.pullers));
  assert(head < MAX_ATTRIBUTES);
  GPUVertexPuller* puller =
      (GPUVertexPuller*)vector_at(&gpu->pullers.pullers, pullerId);
  puller->heads[head].enabled = 0;
}

/**
 * @brief This functions activates vertex puller.
 *
 * @param gpu gpu
 * @param id vertex puller id
 */
void cpu_bindVertexPuller(GPU* const gpu, VertexPullerID id)
{
  assert(gpu != NULL);
  gpu_pushCommand1(gpu,COMMAND_BIND_PULLER,id);
}

/**
 * @brief This function deactivates vertex puller
 *
 * @param gpu gpu
 */
void cpu_unbindVertexPuller(GPU* const gpu)
{
  assert(gpu != NULL);
  gpu_pushCommand(gpu,COMMAND_UNBIND_PULLER);
}

/**
 * @brief This function check if id points to valid vertex puller.
 *
 * @param gpu gpu
 * @param id vertex puller id
 *
 * @return 1 if id points to valid puller, 0 otherwise
 */
int cpu_isVertexPuller(GPU const*const gpu,VertexPullerID id){
  assert(gpu != NULL);
  return gpu_isPuller(gpu,id);
}

/**
 * @brief This function reserves new shader program id.
 *
 * @param gpu gpu
 *
 * @return new program id
 */
ProgramID cpu_createProgram(GPU* const gpu)
{
  assert(gpu != NULL);

  ProgramID id = ifFreeIdExistReturn(&gpu->programs.freeIds);
  if(id != EMPTY_ID)return id;

  GPUProgram newElement;
  gpu_initProgram(&newElement);

  return addToVectorAndReturnIndex(&gpu->programs.programs, &newElement);
}

/**
 * @brief This function attaches shaders to shader program.
 *
 * @param gpu gpu
 * @param programId selected program
 * @param vs vertex shader function pointer
 * @param fs fragment shader function pointer
 *
 */
void cpu_attachShaders(GPU* const     gpu,
                       ProgramID      programId,
                       VertexShader   vs,
                       FragmentShader fs)
{
  assert(cpu_isProgram(gpu,programId));
  GPUProgram* program =
      (GPUProgram*)vector_at(&gpu->programs.programs, programId);
  program->vertexShader   = vs;
  program->fragmentShader = fs;
}

/**
 * @brief This function sets type of attributes that are interpolated by rasterizer.
 *
 * @param gpu gpu
 * @param programId program id
 * @param attrib attribute index
 * @param type type of the attribute
 */
void cpu_setVS2FSType(GPU*const gpu,ProgramID programId,uint32_t attrib,AttributeType type){
  assert(cpu_isProgram(gpu,programId));
  assert(attrib < MAX_ATTRIBUTES);
  GPUProgram* program = 
    (GPUProgram*)vector_at(&gpu->programs.programs, programId);
  program->vs2fsType[attrib] = type;
}

/**
 * @brief This function deletes shader program.
 *
 * @param gpu gpu
 * @param programId shader program id
 *
 */
void cpu_deleteProgram(GPU* const gpu, ProgramID programId)
{
  assert(gpu != NULL);
  assert(programId < vector_size(&gpu->programs.programs));
  GPUProgram* program =
      (GPUProgram*)vector_at(&gpu->programs.programs, programId);
  gpu_initProgram(program);
  stack_push(&gpu->programs.freeIds, &programId);
}

/**
 * @brief This function check if id points to valid program.
 *
 * @param gpu gpu
 * @param id vertex puller id
 *
 * @return 1 if id points to valid program, 0 otherwise
 */
int cpu_isProgram(GPU const*const gpu,ProgramID id){
  assert(gpu != NULL);
  return gpu_isProgram(gpu,id);
}

/**
 * @brief This function sets uniform variable for selected program
 *
 * @param gpu gpu
 * @param programId program id
 * @param uniformId uniform id
 * @param d data
 */
void cpu_programUniform1f(GPU* const  gpu,
                          ProgramID   programId,
                          uint32_t    uniformId,
                          float const d)
{
  GPUUniform* uniform    = gpu_getUniform(gpu, programId, uniformId);
  *(float*)uniform->data = d;
  uniform->type          = UNIFORM_FLOAT;
  uniform->active        = 1;
}

/**
 * @brief This function sets uniform variable for selected program
 *
 * @param gpu gpu
 * @param programId program id
 * @param uniformId uniform id
 * @param d data
 */
void cpu_programUniform2f(GPU* const gpu,
                          ProgramID  programId,
                          uint32_t   uniformId,
                          Vec2 const d)
{
  GPUUniform* uniform = gpu_getUniform(gpu, programId, uniformId);
  copy_Vec2((Vec2*)uniform->data, &d);
  uniform->type   = UNIFORM_VEC2;
  uniform->active = 1;
}

/**
 * @brief This function sets uniform variable for selected program
 *
 * @param gpu gpu
 * @param programId program id
 * @param uniformId uniform id
 * @param d data
 */
void cpu_programUniform3f(GPU* const gpu,
                          ProgramID  programId,
                          uint32_t   uniformId,
                          Vec3 const d)
{
  GPUUniform* uniform = gpu_getUniform(gpu, programId, uniformId);
  copy_Vec3((Vec3*)uniform->data, &d);
  uniform->type   = UNIFORM_VEC3;
  uniform->active = 1;
}

/**
 * @brief This function sets uniform variable for selected program
 *
 * @param gpu gpu
 * @param programId program id
 * @param uniformId uniform id
 * @param d data
 */
void cpu_programUniform4f(GPU* const gpu,
                          ProgramID  programId,
                          uint32_t   uniformId,
                          Vec4 const d)
{
  GPUUniform* uniform = gpu_getUniform(gpu, programId, uniformId);
  copy_Vec4((Vec4*)uniform->data, &d);
  uniform->type   = UNIFORM_VEC4;
  uniform->active = 1;
}

/**
 * @brief This function sets uniform variable for selected program
 *
 * @param gpu gpu
 * @param programId program id
 * @param uniformId uniform id
 * @param d data
 */
void cpu_programUniformMatrix4f(GPU* const gpu,
                                ProgramID  programId,
                                uint32_t   uniformId,
                                Mat4 const d)
{
  GPUUniform* uniform = gpu_getUniform(gpu, programId, uniformId);
  copy_Mat4((Mat4*)uniform->data, &d);
  uniform->type   = UNIFORM_MAT4;
  uniform->active = 1;
}

/**
 * @brief This function sets clearing color of framebuffer
 *
 * @param gpu gpu
 * @param color clear color
 */
void cpu_clearColor(GPU* const gpu, Vec4 color)
{
  assert(gpu != NULL);
  copy_Vec4(&gpu->clearColor, &color);
}

/**
 * @brief This function sets clearing depth of framebuffer
 *
 * @param gpu gpu
 * @param depth clear depth
 */
void cpu_clearDepth(GPU* const gpu, float depth)
{
  assert(gpu != NULL);
  gpu->clearDepth = depth;
}

/**
 * @brief This function inserts draw points command into gpu's command queue
 *
 * @param gpu gpu
 * @param nofVertices number of vertices
 */
void cpu_drawPoints(GPU* const gpu, uint32_t nofVertices)
{
  assert(gpu != NULL);
  gpu_pushCommand2(gpu,COMMAND_DRAW,DRAW_POINTS,nofVertices);
}


/**
 * @brief This function inserts draw triangles command into gpu's command queue
 *
 * @param gpu gpu
 * @param nofVertices number of vertices
 */
void cpu_drawTriangles(GPU* const gpu, uint32_t nofVertices)
{
  assert(gpu != NULL);
  gpu_pushCommand2(gpu,COMMAND_DRAW,DRAW_TRIANGLES,nofVertices);
}

/**
 * @brief This function inserts clear command into gpu's command queue
 *
 * @param gpu gpu
 */
void cpu_clear(GPU* const gpu)
{
  assert(gpu != NULL);
  gpu_pushCommand(gpu,COMMAND_CLEAR);
}

/**
 * @brief This function executes all commands in command queue of the gpu
 *
 * @param gpu gpu
 */
void cpu_finish(GPU* const gpu)
{
  assert(gpu != NULL);
  gpu_finish(gpu);
}

/**
 * @brief This function swaps GPU framebuffer to SDLsurface
 *
 * @param surface SDL surface
 * @param gpu gpu
 */
void cpu_swapBuffers(SDL_Surface* const surface, GPU* const gpu)
{
  uint32_t const bitsPerByte    = 8;
  uint32_t const swizzleTable[] = {
      surface->format->Rshift / bitsPerByte,
      surface->format->Gshift / bitsPerByte,
      surface->format->Bshift / bitsPerByte,
  };
  cpu_finish(gpu);
  GPUFramebuffer* framebuffer = &gpu->framebuffer;
  size_t const    w           = (size_t)surface->w;
  size_t const    h           = (size_t)surface->h;
  uint8_t* const  pixels      = (uint8_t*)surface->pixels;
  for (size_t y = 0; y < h; ++y) {
    size_t const reversedY = h - y - 1;
    size_t const rowStart  = reversedY * w;
    for (size_t x = 0; x < w; ++x) {
      Vec4 const* const color = framebuffer->color + x + y * framebuffer->width;
      uint8_t* const    dstPixel =
          pixels + reversedY * surface->pitch + x * surface->format->BytesPerPixel;
      for (uint32_t c = 0; c < 3; ++c)
        dstPixel[swizzleTable[c]] = gpu_floatColorToUint32(color->data[c]);
    }
  }
}
