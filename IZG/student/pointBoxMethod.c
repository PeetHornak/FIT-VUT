#include <student/emptyMethod.h>
#include <student/application.h>
#include <student/cpu.h>
#include <student/globals.h>

/**
 * @brief This struct holds all variables
 */
struct PointBoxData{
  ProgramID prg;///< program id
  VertexPullerID vao;///< vertex puller id
  BufferID vbo; ///< vertex buffer object id
  uint32_t nofPoints; ///< number of vertices
}pointBoxData;///< this holds all variables of point box method

void pointBox_VS(GPUVertexShaderData*const data){
  //read vertex attribute
  Vec4*pos = (Vec4*)data->inVertex.attributes[0].data;
  Vec4*col = (Vec4*)data->inVertex.attributes[1].data;
  Mat4*viewMatrix       = (Mat4*)data->uniforms->uniform[0].data;
  Mat4*projectionMatrix = (Mat4*)data->uniforms->uniform[1].data;
 
  Mat4 mvp;
  multiply_Mat4_Mat4(&mvp,projectionMatrix,viewMatrix);

  Vec4 vPos;
  multiply_Mat4_Vec4(&vPos,&mvp,pos);

  copy_Vec4(&data->outVertex.gl_Position,&vPos);
  copy_Vec4((Vec4*)data->outVertex.attributes[0].data,col);
}

void pointBox_FS(GPUFragmentShaderData*const data){
  Vec4*col = (Vec4*)data->inFragment.attributes[0].data;
  copy_Vec4(&data->outFragment.gl_FragColor,col);
}

void pointBox_onInit(void*a){
  GPU*gpu = (GPU*)a;
  Vec4 clear;
  init_Vec4(&clear,.0f,.0f,.0f,1.f);
  cpu_clearColor(gpu,clear);

#define pointsOnOneEdge 10

  struct Vertex{
    Vec4 position;
    Vec4 color;
  };
  struct Vertex vertices[pointsOnOneEdge*pointsOnOneEdge*pointsOnOneEdge];
  float const scale = 0.3f;
  for(uint32_t z=0;z<pointsOnOneEdge;++z)
    for(uint32_t y=0;y<pointsOnOneEdge;++y)
      for(uint32_t x=0;x<pointsOnOneEdge;++x){
        struct Vertex*const vert = vertices + (z*pointsOnOneEdge+ y)*pointsOnOneEdge + x;
        float const xf = (float)x/(float)(pointsOnOneEdge-1);
        float const yf = (float)y/(float)(pointsOnOneEdge-1);
        float const zf = (float)z/(float)(pointsOnOneEdge-1);
        vert->position.data[0] = (xf*2.f-1.f)*scale;
        vert->position.data[1] = (yf*2.f-1.f)*scale;
        vert->position.data[2] = (zf*2.f-1.f)*scale;
        vert->position.data[3] = 1;

        vert->color.data[0] = xf;
        vert->color.data[1] = yf;
        vert->color.data[2] = zf;
        vert->color.data[3] = 1;

      }
  pointBoxData.nofPoints = pointsOnOneEdge*pointsOnOneEdge*pointsOnOneEdge;

#undef pointsOnOneEdge

  pointBoxData.vbo = cpu_createBuffer(gpu);
  cpu_bufferData(gpu,pointBoxData.vbo,sizeof(vertices),vertices);


  pointBoxData.vao = cpu_createVertexPuller(gpu);

  cpu_setVertexPuller(gpu,pointBoxData.vao,0,
      ATTRIBUTE_VEC4,sizeof(struct Vertex),0,pointBoxData.vbo);
  cpu_enableVertexPullerHead(gpu,pointBoxData.vao,0);

  cpu_setVertexPuller(gpu,pointBoxData.vao,1,
      ATTRIBUTE_VEC4,sizeof(struct Vertex),sizeof(Vec4),pointBoxData.vbo);
  cpu_enableVertexPullerHead(gpu,pointBoxData.vao,1);



  pointBoxData.prg = cpu_createProgram(gpu);
  cpu_attachShaders(gpu,pointBoxData.prg,pointBox_VS,pointBox_FS);

  cpu_setVS2FSType(gpu,pointBoxData.prg,0,ATTRIBUTE_VEC4);
}

void pointBox_onDraw(void*a){
  GPU*gpu = (GPU*)a;
  cpu_clear(gpu);

  cpu_bindVertexPuller(gpu,pointBoxData.vao);
  cpu_useProgram(gpu,pointBoxData.prg);
  cpu_programUniformMatrix4f(gpu,pointBoxData.prg,0,viewMatrix);
  cpu_programUniformMatrix4f(gpu,pointBoxData.prg,1,projectionMatrix);
  cpu_drawPoints(gpu,pointBoxData.nofPoints);

  cpu_unbindVertexPuller(gpu);
}

void pointBox_onExit(void*a){
  GPU*gpu = (GPU*)a;

  cpu_deleteProgram(gpu,pointBoxData.prg);
  cpu_deleteVertexPuller(gpu,pointBoxData.vao);
  cpu_deleteBuffer(gpu,pointBoxData.vbo);
}
