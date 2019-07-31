#include <student/emptyMethod.h>
#include <student/application.h>
#include <student/cpu.h>
#include <student/globals.h>

/**
 * @brief This struct holds all variables for triangle method
 */
struct TriangleData{
  ProgramID prg;///< id of program
  VertexPullerID vao;///< id of vertex puller
}triangleData;

void triangle_VS(GPUVertexShaderData*const data){
  if(data->inVertex.gl_VertexID == 0)
    init_Vec4(&data->outVertex.gl_Position,-1,-1,0,1);
  if(data->inVertex.gl_VertexID == 1)
    init_Vec4(&data->outVertex.gl_Position,1,-1,0,1);
  if(data->inVertex.gl_VertexID == 2)
    init_Vec4(&data->outVertex.gl_Position,-1,+1,0,1);
}

void triangle_FS(GPUFragmentShaderData*const data){
  init_Vec4(&data->outFragment.gl_FragColor,1,1,1,1);
}

void triangle_onInit(void*a){
  GPU*gpu = (GPU*)a;
  Vec4 clear;
  init_Vec4(&clear,.0f,.0f,.0f,1.f);
  cpu_clearColor(gpu,clear);

  triangleData.vao = cpu_createVertexPuller(gpu);
  triangleData.prg = cpu_createProgram(gpu);
  cpu_attachShaders(gpu,triangleData.prg,triangle_VS,triangle_FS);
}

void triangle_onDraw(void*a){
  GPU*gpu = (GPU*)a;
  cpu_clear(gpu);

  cpu_bindVertexPuller(gpu,triangleData.vao);
  cpu_useProgram(gpu,triangleData.prg);
  cpu_drawTriangles(gpu,3);

  cpu_unbindVertexPuller(gpu);
}

void triangle_onExit(void*a){
  GPU*gpu = (GPU*)a;

  cpu_deleteProgram(gpu,triangleData.prg);
  cpu_deleteVertexPuller(gpu,triangleData.vao);
}
