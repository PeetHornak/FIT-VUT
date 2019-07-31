/*!
 * @file
 * @brief This file contains implementation of software GPU HW.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <student/gpu.h>


/**
 * @brief This functions inits vertex puller head.
 *
 * @param head head
 */
void gpu_initVertexPullerHead(GPUVertexPullerHead *const head){
  assert(head != NULL);
  head->stride   = 0             ;
  head->offset   = 0             ;
  head->bufferId = EMPTY_ID      ;
  head->enabled  = 0             ;
  head->type     = ATTRIBUTE_VEC4;
}

/**
 * @brief This function inits indices
 *
 * @param i indices
 */
void gpu_initIndices(GPUIndices *const i){
  assert(i != NULL);
  i->bufferId = EMPTY_ID;
  i->type     = UINT32  ;
}

/**
 * @brief This function inits vertex puller.
 *
 * @param puller puller
 */
void gpu_initVertexPuller(GPUVertexPuller *const puller){
  assert(puller != NULL);
  for(uint32_t h=0;h<MAX_ATTRIBUTES;++h)
    gpu_initVertexPullerHead(puller->heads+h);
  gpu_initIndices(&puller->indices);
}

/**
 * @brief This function activates selected vertex puller
 *
 * @param gpu gpu
 * @param id vertex puller id
 */
void gpu_activatePuller(GPU *const gpu,VertexPullerID id){
  assert(gpu != NULL);
  assert(id < vector_size(&gpu->pullers.pullers));
  gpu->activeVertexPuller = id;
}

/**
 * @brief This function deactivates vertex puller
 * 
 * @param gpu gpu
 */
void gpu_deactivatePuller(GPU *const gpu){
  assert(gpu != NULL);
  gpu->activeVertexPuller = EMPTY_ID;
}

/**
 * @brief This function check if id points to valid vertex puller.
 *
 * @param gpu gpu
 * @param id vertex puller id
 *
 * @return 1 if id points to valid puller, 0 otherwise
 */
int gpu_isPuller(GPU const*const gpu,VertexPullerID id){
  assert(gpu != NULL);
  if(id == EMPTY_ID)return 0;
  if(id >= vector_size(&gpu->pullers.pullers))return 0;
  if(stack_contain(&gpu->pullers.freeIds,&id))return 0;
  return 1;
}

/**
 * @brief This function returns pointer to active vertex puller
 *
 * @param gpu gpu
 *
 * @return vertex puller pointer
 */
GPUVertexPuller const*gpu_getActivePuller(GPU const*const gpu){
  assert(gpu != NULL);
  assert(gpu_isPuller(gpu,gpu->activeVertexPuller));
  return (GPUVertexPuller const*)vector_atConst(&gpu->pullers.pullers,(uint32_t)gpu->activeVertexPuller);
}

/**
 * @brief This function inits GPU buffer.
 *
 * @param b buffer
 */
void gpu_initBuffer(GPUBuffer*const b){
  b->data = NULL;
  b->size = 0;
}

/**
 * @brief This function check if id points to valid buffer
 *
 * @param gpu gpu
 * @param id buffer id
 *
 * @return 1 if id points to buffer, 0 otherwise
 */
int gpu_isBuffer(GPU const*const gpu,BufferID id){
  assert(gpu != NULL);
  if(id == EMPTY_ID)return 0;
  if(id >= vector_size(&gpu->buffers.buffers))return 0;
  if(stack_contain(&gpu->buffers.freeIds,&id))return 0;
  return 1;
}

/**
 * @brief This function returns pointer to buffer
 *
 * @param gpu gpu
 * @param id id of buffer
 *
 * @return pointer to buffer
 */
GPUBuffer const*gpu_getBuffer(GPU const*const gpu,BufferID id){
  assert(gpu != NULL);
  assert(gpu_isBuffer(gpu,id));
  return (GPUBuffer const*)vector_atConst(&gpu->buffers.buffers,id);
}

/**
 * @brief This function frees all gpu buffers
 *
 * @param b buffers
 */
void gpu_freeBuffers(GPUBuffers*const b){
  uint32_t n = vector_size(&b->buffers);
  for(uint32_t i=0;i<n;++i)
    gpu_freeBuffer(vector_at(&b->buffers,i));
  vector_free(&b->buffers);
  stack_free(&b->freeIds);
}

/**
 * @brief This function frees the buffer.
 *
 * @param b buffer
 */
void gpu_freeBuffer(GPUBuffer*const b){
  if(b->data)free(b->data);
  gpu_initBuffer(b);
}

/**
 * @brief This function allocates and uploads data to buffer
 *
 * @param b buffer
 * @param size size of new buffer
 * @param data optional data pointer
 */
void gpu_bufferData(GPUBuffer*const b,uint64_t size,void*data){
  gpu_freeBuffer(b);
  b->data = malloc(size);
  b->size = size;
  if(data)
    memcpy(b->data,data,size);
}

/**
 * @brief This function inits GPU buffers.
 *
 * @param b biffers
 */
void gpu_initBuffers(GPUBuffers *const b){
  assert(b != NULL);
  vector_init(&b->buffers,sizeof(GPUBuffer));
  stack_init(&b->freeIds,sizeof(uint32_t));
}

/**
 * @brief This function init vertex pullers.
 *
 * @param v vertex pullers
 */
void gpu_initVertexPullers(GPUVertexPullers *const v){
  assert(v != NULL);
  vector_init(&v->pullers,sizeof(GPUVertexPuller));
  stack_init(&v->freeIds,sizeof(uint32_t));
}

/**
 * @brief This function free all vertex pullers
 *
 * @param v vertex pullers
 */
void gpu_freeVertexPullers(GPUVertexPullers *const v){
  assert(v != NULL);
  vector_free(&v->pullers);
  stack_free(&v->freeIds);
}

/**
 * @brief This function inits one uniform variable.
 *
 * @param u uniform variable
 */
void gpu_initUniform(GPUUniform* const u){
  u->active = 0;
  u->type   = UNIFORM_VEC4;
  memset((void*)u->data,0,sizeof(u->data));
}

/**
 * @brief This function inits uniform variables
 *
 * @param u uniform variables
 */
void gpu_initUniforms(GPUUniforms* const u){
  for(uint32_t i=0;i<MAX_UNIFORMS;++i)
    gpu_initUniform(u->uniform+i);
}

/**
 * @brief This functions inits shader program.
 *
 * @param p shader program
 */
void gpu_initProgram(GPUProgram* const p){
  p->fragmentShader = NULL;
  p->vertexShader   = NULL;
  gpu_initUniforms(&p->uniforms);
  for(uint32_t a=0;a<MAX_ATTRIBUTES;++a)
    p->vs2fsType[a] = ATTRIBUTE_EMPTY;
}

/**
 * @brief This function inits programs
 *
 * @param p programs
 */
void gpu_initPrograms(GPUPrograms *const p){
  assert(p != NULL);
  vector_init(&p->programs,sizeof(GPUProgram));
  stack_init(&p->freeIds,sizeof(uint32_t));
}

/**
 * @brief This function frees all gpu programs
 *
 * @param p programs
 */
void gpu_freePrograms(GPUPrograms *const p){
  assert(p != NULL);
  vector_free(&p->programs);
  stack_free(&p->freeIds);
}

/**
 * @brief This function activates selected program
 *
 * @param gpu gpu
 * @param id program id
 */
void gpu_activateProgram(GPU* const gpu,ProgramID id){
  assert(gpu != NULL);
  assert(id < vector_size(&gpu->programs.programs));
  gpu->activeProgram = id;
}

/**
 * @brief This function check if id points to valid program.
 *
 * @param gpu gpu
 * @param id vertex puller id
 *
 * @return 1 if id points to valid program, 0 otherwise
 */
int  gpu_isProgram(GPU const*const gpu,ProgramID id){
  assert(gpu != NULL);
  if(id == EMPTY_ID)return 0;
  if(id >= vector_size(&gpu->programs.programs))return 0;
  if(stack_contain(&gpu->programs.freeIds,&id))return 0;
  return 1;
}

/**
 * @brief This function returns pointer to active program.
 *
 * @param gpu gpu
 *
 * @return pointer to active program or NULL
 */
GPUProgram const*gpu_getActiveProgram(GPU const*const gpu){
  assert(gpu);
  assert(gpu_isProgram(gpu,gpu->activeProgram));
  return (GPUProgram const*)vector_atConst(&gpu->programs.programs,gpu->activeProgram);
}

/**
 * @brief This function pushes command to gpu's command buffer
 *
 * @param gpu gpu 
 * @param type command type
 */
void gpu_pushCommand(GPU* const gpu,GPUCommandType type){
  GPUCommand command;
  command.type = type;
  queue_push(&gpu->commandQueue, &command);
}

/**
 * @brief This function pushes command to gpu's command buffer
 *
 * @param gpu gpu 
 * @param type command type
 * @param p0 first parameter of command
 */
void gpu_pushCommand1(GPU* const gpu,GPUCommandType type,uint64_t p0){
  GPUCommand command;
  command.type = type;
  command.parameters[0] = p0;
  queue_push(&gpu->commandQueue, &command);
}

/**
 * @brief This function pushes command to gpu's command buffer
 *
 * @param gpu gpu 
 * @param type command type
 * @param p0 first parameter of command
 * @param p1 second parameter of command
 */
void gpu_pushCommand2(GPU* const gpu,GPUCommandType type,uint64_t p0,uint64_t p1){
  GPUCommand command;
  command.type = type;
  command.parameters[0] = p0;
  command.parameters[1] = p1;
  queue_push(&gpu->commandQueue, &command);
}

/**
 * @brief This function inits framebuffer.
 *
 * @param f framebuffer
 */
void gpu_initFramebuffer(GPUFramebuffer *const f){
  assert(f != NULL);
  f->color = NULL;
  f->depth = NULL;
  f->width = 0;
  f->height = 0;
}

/**
 * @brief This function allocates framebuffer
 *
 * @param f framebuffer
 * @param width width of the framebuffer
 * @param height height of the framebuffer
 */
void gpu_allocateFramebuffer(GPUFramebuffer *const f,uint32_t width,uint32_t height){
  uint32_t nofPixels = width*height;
  f->color = (Vec4*)malloc(sizeof(Vec4)*nofPixels);
  f->depth = (float*)malloc(sizeof(float)*nofPixels);
  f->width = width;
  f->height = height;
  for(uint32_t y=0;y<height;++y)
    for(uint32_t x=0;x<width;++x){
      f->depth[y*width+x] = 1.f;
      init_Vec4(f->color + y*width+x,0,0,0,0);
    }
}

/**
 * @brief This function frees the framebuffer.
 *
 * @param f framebuffer
 */
void gpu_freeFramebuffer(GPUFramebuffer *const f){
  if(f->color)free(f->color);
  if(f->depth)free(f->depth);
  gpu_initFramebuffer(f);
}

/**
 * @brief This function clears framebuffer
 *
 * @param gpu framebuffer
 */
void gpu_clearFramebuffer(GPU* const gpu){
  assert(gpu != NULL);
  uint32_t const nofPixels = gpu->framebuffer.width * gpu->framebuffer.height;
  for(uint32_t i=0;i<nofPixels;++i){
    copy_Vec4(gpu->framebuffer.color+i,&gpu->clearColor);
    gpu->framebuffer.depth[i] = gpu->clearDepth;
  }
}

/**
 * @brief This function executes all commands in gpu's command queue
 *
 * @param gpu gpu
 */
void gpu_finish(GPU* const gpu){
  assert(gpu != NULL);
  while (!queue_empty(&gpu->commandQueue)) {
    GPUCommand command;
    queue_pop(&gpu->commandQueue, &command);
    if (command.type == COMMAND_DRAW) {
      GPUDrawType type = (GPUDrawType)command.parameters[0];
      uint32_t nofVertices = (uint32_t)command.parameters[1];
      if(gpu->debug)
        gpu_checkDraw(gpu,nofVertices);
      if(type == DRAW_TRIANGLES)
        gpu_drawTriangles(gpu, nofVertices);
      if(type == DRAW_POINTS)
        gpu_drawPoints(gpu, nofVertices);
    }
    if (command.type == COMMAND_CLEAR) {
      gpu_clearFramebuffer(gpu);
    }
    if (command.type == COMMAND_USE_PROGRAM) {
      ProgramID id = (ProgramID)command.parameters[0];
      gpu_activateProgram(gpu,id);
    }
    if (command.type == COMMAND_BIND_PULLER) {
      VertexPullerID id = (VertexPullerID)command.parameters[0];
      gpu_activatePuller(gpu,id);
    }
    if (command.type == COMMAND_UNBIND_PULLER) {
      gpu_deactivatePuller(gpu);
    }
  }
}

/**
 * @brief This function returns pointer to selected uniform.
 *
 * @param gpu gpu
 * @param programId program id
 * @param uniformId uniform id
 *
 * @return 
 */
GPUUniform* gpu_getUniform(GPU* const gpu,ProgramID programId,uint32_t uniformId){
  assert(gpu != NULL);
  assert(programId < vector_size(&gpu->programs.programs));
  assert(uniformId < MAX_UNIFORMS);
  GPUProgram*program = (GPUProgram*)vector_at(&gpu->programs.programs,programId);
  return program->uniforms.uniform+uniformId;
}

/**
 * @brief This function converts float [0,1] to byte [0,255]
 *
 * @param value float value in range [0,1]
 *
 * @return byte value in range [0,255]
 */
uint8_t gpu_floatColorToUint32(float const value) {
  return (uint8_t)(value * 255.f);
}

/**
 * @brief This function check gpu state for draw call.
 *
 * @param gpu gpu
 * @param nofVertices number of vertices
 */
void gpu_checkDraw(GPU const*const gpu,uint32_t nofVertices){
  if(gpu == NULL){
    fprintf(stderr,"Cannot draw - GPU == NULL!\n");
    exit(0);
  }

  if(!gpu_isPuller(gpu,gpu->activeVertexPuller)){
    fprintf(stderr,"Cannot draw - activated vertex puller id does not point to valid vertex puller!\n");
    exit(0);
  }

  if(!gpu_isProgram(gpu,gpu->activeProgram)){
    fprintf(stderr,"Cannot draw - activated program id does not point to valid shader program\n");
    exit(0);
  }

  GPUProgram const*const prg = gpu_getActiveProgram(gpu);

  if(prg->vertexShader == NULL){
    fprintf(stderr,"Cannot draw - activated program does not contain valid vertex shader\n");
    exit(0);
  }

  if(prg->fragmentShader == NULL){
    fprintf(stderr,"Cannot draw - activated program does not contain valid fragment shader\n");
    exit(0);
  }

  for(uint32_t a=0;a<MAX_ATTRIBUTES;++a){
    AttributeType type = prg->vs2fsType[a];
    if(
      type != ATTRIBUTE_EMPTY &&
      type != ATTRIBUTE_FLOAT &&
      type != ATTRIBUTE_VEC2  &&
      type != ATTRIBUTE_VEC3  &&
      type != ATTRIBUTE_VEC4
      ){
      fprintf(stderr,
          "Cannot draw - activated program has bad vertex shader to fragment shader attribute type (%i) for attribute %i!\n",type,a);
      exit(0);
    }
  }

  GPUVertexPuller const*const vao = gpu_getActivePuller(gpu);

  uint32_t maxVertexID = 0;
  if(vao->indices.bufferId != EMPTY_ID){
    if(!gpu_isBuffer(gpu,vao->indices.bufferId)){
      fprintf(stderr,"Cannot draw - index buffer id does not point to valid buffer!\n");
      exit(0);
    }
    GPUBuffer const*const indices = (GPUBuffer const*)vector_atConst(&gpu->buffers.buffers,vao->indices.bufferId);
    uint32_t indSize = (uint32_t)vao->indices.type;
    if(indSize != 1 && indSize != 2 && indSize != 4){
      fprintf(stderr,"Cannot draw - index type is not supported (UIN8, UINT16, UINT32)!\n");
      exit(0);
    }
    if(indSize*(nofVertices-1) >= indices->size){
      fprintf(stderr,"Cannot draw - vertex index buffer is too small (size %u) or number of vertices in draw call is to high (%u) or index size (%u) is wrong!\n",
          (uint32_t)indices->size,(uint32_t)nofVertices,(uint32_t)indSize);
      exit(0);
    }
    for(uint32_t i=0;i<nofVertices;++i){
      uint8_t const*const ptr = (uint8_t*)indices->data;
      uint32_t vertexId = 0;
      for(uint32_t b=0;b<indSize;++b)
        vertexId += ptr[i*indSize+b] << (8*b);
      if(vertexId > maxVertexID)
        maxVertexID = vertexId;
    }
  }else{
    maxVertexID = nofVertices-1;
  }

  for(uint32_t h=0;h<MAX_ATTRIBUTES;++h){
    if(!vao->heads[h].enabled)continue;

    BufferID bi = vao->heads[h].bufferId;

    if(!gpu_isBuffer(gpu,bi)){
      fprintf(stderr,"Cannot draw - vertex puller head %i use non existing vertex buffer %u !\n",h,(uint32_t)bi);
      exit(0);
    }

    GPUBuffer const*const buf = gpu_getBuffer(gpu,bi);

    uint64_t offset = vao->heads[h].offset;
    uint64_t stride = vao->heads[h].stride;
    uint64_t size   = (uint64_t)vao->heads[h].type;
    if(offset + stride*maxVertexID + size > buf->size){
      fprintf(stderr,"Cannot draw - vertex puller head %i (offset=%u stride=%u) would read outside of the scope of buffer %u (size %u)\n!",
          h,(uint32_t)offset,(uint32_t)stride,(uint32_t)bi,(uint32_t)buf->size);
      exit(0);
    }
  }
}
