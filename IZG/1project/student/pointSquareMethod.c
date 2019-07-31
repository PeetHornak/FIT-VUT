#include <student/emptyMethod.h>
#include <student/application.h>
#include <student/cpu.h>

/**
 * @brief This holds all variables
 */
struct PointSquareData{
  ProgramID prg;///<program id
  VertexPullerID vao;///< vertex puller id
  BufferID vbo; ///< vertex buffer id
  uint32_t nofPoints; ///< number of vertices
}pointSquareData;///< holds all variables

void pointSquare_VS(GPUVertexShaderData*const data){
  //read vertex attribute
  Vec4*pos = (Vec4*)data->inVertex.attributes[0].data;
  Vec4*col = (Vec4*)data->inVertex.attributes[1].data;

  copy_Vec4(&data->outVertex.gl_Position,pos);
  copy_Vec4((Vec4*)data->outVertex.attributes[0].data,col);
}

void pointSquare_FS(GPUFragmentShaderData*const data){
  Vec4*col = (Vec4*)data->inFragment.attributes[0].data;
  copy_Vec4(&data->outFragment.gl_FragColor,col);
}

void pointSquare_onInit(void*a){
  GPU*gpu = (GPU*)a;
  Vec4 clear;
  init_Vec4(&clear,.0f,.0f,.0f,1.f);
  cpu_clearColor(gpu,clear);

#define pointsOnOneEdge 20

  struct Vertex{
    Vec4 position;
    Vec4 color;
  };
  struct Vertex vertices[pointsOnOneEdge*pointsOnOneEdge];
  float const scale = 0.3f;
  for(uint32_t y=0;y<pointsOnOneEdge;++y)
    for(uint32_t x=0;x<pointsOnOneEdge;++x){
      struct Vertex*const vert = vertices + y*pointsOnOneEdge + x;
      float const xf = (float)x/(float)(pointsOnOneEdge-1);
      float const yf = (float)y/(float)(pointsOnOneEdge-1);
      vert->position.data[0] = (xf*2.f-1.f)*scale;
      vert->position.data[1] = (yf*2.f-1.f)*scale;
      vert->position.data[2] = 0;
      vert->position.data[3] = 1;

      vert->color.data[0] = xf;
      vert->color.data[1] = yf;
      vert->color.data[2] = 0;
      vert->color.data[3] = 1;

    }
  pointSquareData.nofPoints = pointsOnOneEdge*pointsOnOneEdge;

#undef pointsOnOneEdge

  pointSquareData.vbo = cpu_createBuffer(gpu);
  cpu_bufferData(gpu,pointSquareData.vbo,sizeof(vertices),vertices);


  pointSquareData.vao = cpu_createVertexPuller(gpu);

  cpu_setVertexPuller(gpu,pointSquareData.vao,0,
      ATTRIBUTE_VEC4,sizeof(struct Vertex),0,pointSquareData.vbo);
  cpu_enableVertexPullerHead(gpu,pointSquareData.vao,0);

  cpu_setVertexPuller(gpu,pointSquareData.vao,1,
      ATTRIBUTE_VEC4,sizeof(struct Vertex),sizeof(Vec4),pointSquareData.vbo);
  cpu_enableVertexPullerHead(gpu,pointSquareData.vao,1);



  pointSquareData.prg = cpu_createProgram(gpu);
  cpu_attachShaders(gpu,pointSquareData.prg,pointSquare_VS,pointSquare_FS);

  cpu_setVS2FSType(gpu,pointSquareData.prg,0,ATTRIBUTE_VEC4);
}

void pointSquare_onDraw(void*a){
  GPU*gpu = (GPU*)a;
  cpu_clear(gpu);

  cpu_bindVertexPuller(gpu,pointSquareData.vao);
  cpu_useProgram(gpu,pointSquareData.prg);
  cpu_drawPoints(gpu,pointSquareData.nofPoints);

  cpu_unbindVertexPuller(gpu);
}

void pointSquare_onExit(void*a){
  GPU*gpu = (GPU*)a;

  cpu_deleteProgram(gpu,pointSquareData.prg);
  cpu_deleteVertexPuller(gpu,pointSquareData.vao);
  cpu_deleteBuffer(gpu,pointSquareData.vbo);
}
