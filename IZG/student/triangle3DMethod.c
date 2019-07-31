#include <student/emptyMethod.h>
#include <student/application.h>
#include <student/cpu.h>
#include <student/globals.h>

/**
 * @brief This struct holds all variables of triangle3D method
 */
struct Triangle3dData{
  ProgramID prg;///< program id
  VertexPullerID vao;///< vertex puller id
}triangle3dData;///< this variable holds all triangle3D vars

void triangle3d_VS(GPUVertexShaderData*const data){
  init_Vec4(&data->outVertex.gl_Position,0,0,0,1);
  Mat4*viewMatrix       = (Mat4*)data->uniforms->uniform[3].data;
  Mat4*projectionMatrix = (Mat4*)data->uniforms->uniform[2].data;
 
  Mat4 mvp;
  multiply_Mat4_Mat4(&mvp,projectionMatrix,viewMatrix);

  Vec4 pos;
  Vec4 col;

  if(data->inVertex.gl_VertexID == 0){
    init_Vec4(&pos,-1,-1,0,1);
    init_Vec4(&col,1,0,0,1);
  }
  if(data->inVertex.gl_VertexID == 1){
    init_Vec4(&pos,1,-1,0,1);
    init_Vec4(&col,0,1,0,1);
  }
  if(data->inVertex.gl_VertexID == 2){
    init_Vec4(&pos,-1,+1,0,1);
    init_Vec4(&col,0,0,1,1);
  }

  copy_Vec4((Vec4*)data->outVertex.attributes[3].data,&col);
  multiply_Mat4_Vec4(&data->outVertex.gl_Position,&mvp,&pos);
}

void triangle3d_FS(GPUFragmentShaderData*const data){
  copy_Vec4(&data->outFragment.gl_FragColor,(Vec4*)data->inFragment.attributes[3].data);
}

void triangle3d_onInit(void*a){
  GPU*gpu = (GPU*)a;
  Vec4 clear;
  init_Vec4(&clear,.0f,.0f,.0f,1.f);
  cpu_clearColor(gpu,clear);


  triangle3dData.vao = cpu_createVertexPuller(gpu);
  triangle3dData.prg = cpu_createProgram(gpu);
  cpu_attachShaders(gpu,triangle3dData.prg,triangle3d_VS,triangle3d_FS);
  cpu_setVS2FSType(gpu,triangle3dData.prg,3,ATTRIBUTE_VEC4);
}

void triangle3d_onDraw(void*a){
  GPU*gpu = (GPU*)a;
  cpu_clear(gpu);

  cpu_bindVertexPuller(gpu,triangle3dData.vao);
  cpu_useProgram(gpu,triangle3dData.prg);
  cpu_programUniformMatrix4f(gpu,triangle3dData.prg,3,viewMatrix);
  cpu_programUniformMatrix4f(gpu,triangle3dData.prg,2,projectionMatrix);
  cpu_drawTriangles(gpu,3);

  cpu_unbindVertexPuller(gpu);
}

void triangle3d_onExit(void*a){
  GPU*gpu = (GPU*)a;

  cpu_deleteProgram(gpu,triangle3dData.prg);
  cpu_deleteVertexPuller(gpu,triangle3dData.vao);
}
