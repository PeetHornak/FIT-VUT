#include <student/emptyMethod.h>
#include <student/application.h>
#include <student/cpu.h>

/**
 * @brief This holds all variable of this method
 */
struct PointData{
  ProgramID prg;///< program id
  VertexPullerID vao; ///< vertex puller id
}pointData;///< holds all variables

void point_VS(GPUVertexShaderData*const data){
  init_Vec4(&data->outVertex.gl_Position,0,0,0,1);
}

void point_FS(GPUFragmentShaderData*const data){
  init_Vec4(&data->outFragment.gl_FragColor,1,1,1,1);
}

void point_onInit(void*a){
  GPU*gpu = (GPU*)a;
  Vec4 clear;
  init_Vec4(&clear,.0f,.0f,.0f,1.f);
  cpu_clearColor(gpu,clear);


  pointData.vao = cpu_createVertexPuller(gpu);
  pointData.prg = cpu_createProgram(gpu);
  cpu_attachShaders(gpu,pointData.prg,point_VS,point_FS);
  
}

void point_onDraw(void*a){
  GPU*gpu = (GPU*)a;
  cpu_clear(gpu);

  cpu_bindVertexPuller(gpu,pointData.vao);
  cpu_useProgram(gpu,pointData.prg);
  cpu_drawPoints(gpu,1);

  cpu_unbindVertexPuller(gpu);

}

void point_onExit(void*a){
  GPU*gpu = (GPU*)a;

  cpu_deleteProgram(gpu,pointData.prg);
  cpu_deleteVertexPuller(gpu,pointData.vao);
}
