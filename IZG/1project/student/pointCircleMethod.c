#include <student/emptyMethod.h>
#include <student/application.h>
#include <student/cpu.h>

/**
 * @brief This holds all variables for point circle method
 */
struct PointCircleData{
  ProgramID prg;///< program id
  VertexPullerID vao; ///< vertex puller id
  BufferID vbo; ///< vertex buffer id
  uint32_t nofPoints; ///< number of vertices
}pointCircleData;///< holds all variables

void pointCircle_VS(GPUVertexShaderData*const data){
  //read vertex attribute
  Vec4*pos = (Vec4*)data->inVertex.attributes[0].data;

  copy_Vec4(&data->outVertex.gl_Position,pos);
}

void pointCircle_FS(GPUFragmentShaderData*const data){
  init_Vec4(&data->outFragment.gl_FragColor,1,1,1,1);
}

void pointCircle_onInit(void*a){
  GPU*gpu = (GPU*)a;
  Vec4 clear;
  init_Vec4(&clear,.3f,.0f,.0f,1.f);
  cpu_clearColor(gpu,clear);

#define nPoints 100
  Vec4 vertices[nPoints];
  float const radius = 0.5f;
  for(uint32_t i=0;i<nPoints;++i){
    float const angle = (float)i/(float)nPoints * MY_PI * 2.f;
    vertices[i].data[0] = (float)(cos(angle)*radius);
    vertices[i].data[1] = (float)(sin(angle)*radius);
    vertices[i].data[2] = 0.f;
    vertices[i].data[3] = 1.f;
  }
  pointCircleData.nofPoints = nPoints;
#undef nPoints

  pointCircleData.vbo = cpu_createBuffer(gpu);
  cpu_bufferData(gpu,pointCircleData.vbo,sizeof(vertices),vertices);


  pointCircleData.vao = cpu_createVertexPuller(gpu);
  cpu_setVertexPuller(gpu,pointCircleData.vao,0,
      ATTRIBUTE_VEC4,sizeof(Vec4),0,pointCircleData.vbo);
  cpu_enableVertexPullerHead(gpu,pointCircleData.vao,0);

  pointCircleData.prg = cpu_createProgram(gpu);
  cpu_attachShaders(gpu,pointCircleData.prg,pointCircle_VS,pointCircle_FS);
  
}

void pointCircle_onDraw(void*a){
  GPU*gpu = (GPU*)a;
  cpu_clear(gpu);

  cpu_bindVertexPuller(gpu,pointCircleData.vao);
  cpu_useProgram(gpu,pointCircleData.prg);
  cpu_drawPoints(gpu,pointCircleData.nofPoints);

  cpu_unbindVertexPuller(gpu);
}

void pointCircle_onExit(void*a){
  GPU*gpu = (GPU*)a;

  cpu_deleteProgram(gpu,pointCircleData.prg);
  cpu_deleteVertexPuller(gpu,pointCircleData.vao);
  cpu_deleteBuffer(gpu,pointCircleData.vbo);
}
