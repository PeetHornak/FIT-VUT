#include <3rdParty/catch.hpp>

#include <iostream>
#include <string.h>

#include <algorithm>
#include <numeric>

#include <student/cpu.h>
#include <student/phongMethod.h>
#include <student/globals.h>
#include <tests/groundTruth.h>

float const floatErr = 0.001f;

bool equalFloats(float const& a, float const& b,float err = floatErr) {
  return fabs(a - b) <= err;
}

bool equalCounts(size_t a,size_t b,size_t err = 10){
  if(a<b)return (b-a)<err;
  return (a-b)<err;
}

bool greaterFloat(float a,float b,float err = floatErr){
  return a>b-err;
}

size_t vertexShaderInvocationCounter = 0;
void vertexShaderCounter(GPUVertexShaderData*const v){
  vertexShaderInvocationCounter++;
  init_Vec4(&v->outVertex.gl_Position,0,0,0,1);
}

void fragmentShaderEmpty(GPUFragmentShaderData*const f){(void)f;}

SCENARIO("vertex shader should be executed as many times as the number of vertices in draw call."){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);
  auto vao = cpu_createVertexPuller(&gpu);
  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderCounter,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  vertexShaderInvocationCounter=0;
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,9);
  cpu_finish(&gpu);

  cpu_freeGPU(&gpu);
  REQUIRE(vertexShaderInvocationCounter == 9);
}


std::vector<uint32_t>gl_VertexIds;
void vertexShaderID(GPUVertexShaderData*const v){
  gl_VertexIds.push_back(v->inVertex.gl_VertexID);
  init_Vec4(&v->outVertex.gl_Position,0,0,0,1);
}

SCENARIO("gl_VertexID value should contain vertex number when not using indexing"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);
  auto vao = cpu_createVertexPuller(&gpu);
  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderID,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  uint32_t nofV = 9;

  gl_VertexIds = {};
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,nofV);
  cpu_finish(&gpu);

  std::vector<uint32_t>res(nofV);
  std::iota(std::begin(res),std::end(res),0);
  cpu_freeGPU(&gpu);
  REQUIRE(gl_VertexIds == res);
}

SCENARIO("gl_VertexID value should contain index number when using 32bit indexing"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  BufferID ebo = cpu_createBuffer(&gpu);
  std::vector<uint32_t> indices = {0,1,2,2,1,3};
  cpu_bufferData(&gpu,ebo,indices.size()*sizeof(decltype(indices)::value_type),indices.data());

  auto vao = cpu_createVertexPuller(&gpu);
  cpu_setVertexPullerIndexing(&gpu,vao,UINT32,ebo);

  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderID,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  gl_VertexIds = {};
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,(uint32_t)indices.size());
  cpu_finish(&gpu);

  cpu_freeGPU(&gpu);
  REQUIRE(gl_VertexIds == indices);
}

SCENARIO("gl_VertexID value should contain index number when using 16bit indexing"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  BufferID ebo = cpu_createBuffer(&gpu);
  std::vector<uint16_t> indices = {0,9,8,2,3,9,4,2,1};
  cpu_bufferData(&gpu,ebo,indices.size()*sizeof(decltype(indices)::value_type),indices.data());

  auto vao = cpu_createVertexPuller(&gpu);
  cpu_setVertexPullerIndexing(&gpu,vao,UINT16,ebo);

  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderID,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  gl_VertexIds = {};
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,(uint32_t)indices.size());
  cpu_finish(&gpu);

  std::vector<uint32_t>indCmp;
  for(auto x:indices)
    indCmp.push_back(x);
  cpu_freeGPU(&gpu);
  REQUIRE(gl_VertexIds == indCmp);
}


SCENARIO("gl_VertexID value should contain index number when using 8bit indexing"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  BufferID ebo = cpu_createBuffer(&gpu);
  std::vector<uint8_t> indices = {100,232,123,21,0,12,33,78,56};
  cpu_bufferData(&gpu,ebo,indices.size()*sizeof(decltype(indices)::value_type),indices.data());

  auto vao = cpu_createVertexPuller(&gpu);
  cpu_setVertexPullerIndexing(&gpu,vao,UINT8,ebo);

  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderID,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  gl_VertexIds = {};
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,(uint32_t)indices.size());
  cpu_finish(&gpu);

  std::vector<uint32_t>indCmp;
  for(auto x:indices)
    indCmp.push_back(x);
  cpu_freeGPU(&gpu);
  REQUIRE(gl_VertexIds == indCmp);
}


GPUUniforms const*uniforms = NULL;
void vertexShaderUnif(GPUVertexShaderData*const v){
  uniforms = v->uniforms;
  init_Vec4(&v->outVertex.gl_Position,0,0,0,1);
}


SCENARIO("vertex shader should receive uniforms from active shader program"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  auto vao = cpu_createVertexPuller(&gpu);

  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderUnif,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  gl_VertexIds = {};
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,3);
  cpu_finish(&gpu);

  auto ptr = &gpu_getActiveProgram(&gpu)->uniforms;
  cpu_freeGPU(&gpu);
  REQUIRE(uniforms == ptr);
}

std::vector<GPUInVertex>inVertices;
void vertexShaderVert(GPUVertexShaderData*const v){
  inVertices.push_back(v->inVertex);
  init_Vec4(&v->outVertex.gl_Position,0,0,0,1);
}

SCENARIO("vertex shader should receive correct attributes from buffer"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  std::vector<float> vert = {0.f,1.f,2.f,3.f,4.f,5.f,6.f,7.f,8.f,9.f,10.f,11.f};
  BufferID vbo = cpu_createBuffer(&gpu);
  cpu_bufferData(&gpu,vbo,vert.size()*sizeof(decltype(vert)::value_type),vert.data());

  auto vao = cpu_createVertexPuller(&gpu);
  cpu_setVertexPuller(&gpu,vao,0,ATTRIBUTE_FLOAT,sizeof(float),0,vbo);
  cpu_enableVertexPullerHead(&gpu,vao,0);

  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderVert,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  inVertices.clear();
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,(uint32_t)vert.size());
  cpu_finish(&gpu);

  cpu_freeGPU(&gpu);

  REQUIRE(inVertices.size() == 12);
  REQUIRE(*(float*)inVertices[ 0].attributes[0].data ==  0.f);
  REQUIRE(*(float*)inVertices[ 1].attributes[0].data ==  1.f);
  REQUIRE(*(float*)inVertices[ 2].attributes[0].data ==  2.f);
  REQUIRE(*(float*)inVertices[ 3].attributes[0].data ==  3.f);
  REQUIRE(*(float*)inVertices[ 4].attributes[0].data ==  4.f);
  REQUIRE(*(float*)inVertices[ 5].attributes[0].data ==  5.f);
  REQUIRE(*(float*)inVertices[ 6].attributes[0].data ==  6.f);
  REQUIRE(*(float*)inVertices[ 7].attributes[0].data ==  7.f);
  REQUIRE(*(float*)inVertices[ 8].attributes[0].data ==  8.f);
  REQUIRE(*(float*)inVertices[ 9].attributes[0].data ==  9.f);
  REQUIRE(*(float*)inVertices[10].attributes[0].data == 10.f);
  REQUIRE(*(float*)inVertices[11].attributes[0].data == 11.f);

}

SCENARIO("vertex shader should receive correct attributes from buffer when using offset"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  std::vector<float> vert = {0.f,1.f,2.f,3.f,4.f,5.f,6.f,7.f,8.f,9.f,10.f,11.f};
  BufferID vbo = cpu_createBuffer(&gpu);
  cpu_bufferData(&gpu,vbo,vert.size()*sizeof(decltype(vert)::value_type),vert.data());

  auto vao = cpu_createVertexPuller(&gpu);
  cpu_setVertexPuller(&gpu,vao,0,ATTRIBUTE_FLOAT,sizeof(float),sizeof(float)*3,vbo);
  cpu_enableVertexPullerHead(&gpu,vao,0);

  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderVert,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  inVertices.clear();
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,(uint32_t)vert.size()-3);
  cpu_finish(&gpu);

  cpu_freeGPU(&gpu);

  REQUIRE(inVertices.size() == 9);
  REQUIRE(*(float*)inVertices[ 0].attributes[0].data ==  3.f);
  REQUIRE(*(float*)inVertices[ 1].attributes[0].data ==  4.f);
  REQUIRE(*(float*)inVertices[ 2].attributes[0].data ==  5.f);
  REQUIRE(*(float*)inVertices[ 3].attributes[0].data ==  6.f);
  REQUIRE(*(float*)inVertices[ 4].attributes[0].data ==  7.f);
  REQUIRE(*(float*)inVertices[ 5].attributes[0].data ==  8.f);
  REQUIRE(*(float*)inVertices[ 6].attributes[0].data ==  9.f);
  REQUIRE(*(float*)inVertices[ 7].attributes[0].data == 10.f);
  REQUIRE(*(float*)inVertices[ 8].attributes[0].data == 11.f);

}


SCENARIO("vertex shader should receive correct attributes from buffer when using stride"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  std::vector<float> vert = {0.f,1.f,2.f,3.f,4.f,5.f,6.f,7.f,8.f,9.f,10.f,11.f};
  BufferID vbo = cpu_createBuffer(&gpu);
  cpu_bufferData(&gpu,vbo,vert.size()*sizeof(decltype(vert)::value_type),vert.data());

  auto vao = cpu_createVertexPuller(&gpu);
  cpu_setVertexPuller(&gpu,vao,0,ATTRIBUTE_FLOAT,sizeof(float)*4,0,vbo);
  cpu_enableVertexPullerHead(&gpu,vao,0);

  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderVert,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  inVertices.clear();
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,(uint32_t)vert.size()/4);
  cpu_finish(&gpu);

  cpu_freeGPU(&gpu);

  REQUIRE(inVertices.size() == 3);
  REQUIRE(*(float*)inVertices[ 0].attributes[0].data ==  0.f);
  REQUIRE(*(float*)inVertices[ 1].attributes[0].data ==  4.f);
  REQUIRE(*(float*)inVertices[ 2].attributes[0].data ==  8.f);

}

SCENARIO("vertex shader should receive correct attributes from buffer when using offset and stride"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  std::vector<float> vert = {0.f,1.f,2.f,3.f,4.f,5.f,6.f,7.f,8.f,9.f,10.f,11.f};
  BufferID vbo = cpu_createBuffer(&gpu);
  cpu_bufferData(&gpu,vbo,vert.size()*sizeof(decltype(vert)::value_type),vert.data());

  auto vao = cpu_createVertexPuller(&gpu);
  cpu_setVertexPuller(&gpu,vao,0,ATTRIBUTE_FLOAT,sizeof(float)*4,sizeof(float)*2,vbo);
  cpu_enableVertexPullerHead(&gpu,vao,0);

  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderVert,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  inVertices.clear();
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,(uint32_t)vert.size()/4);
  cpu_finish(&gpu);

  cpu_freeGPU(&gpu);

  REQUIRE(inVertices.size() == 3);
  REQUIRE(*(float*)inVertices[ 0].attributes[0].data ==   2.f);
  REQUIRE(*(float*)inVertices[ 1].attributes[0].data ==   6.f);
  REQUIRE(*(float*)inVertices[ 2].attributes[0].data ==  10.f);

}

SCENARIO("vertex shader should receive correct attributes when using multiple buffers"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  uint8_t b0[400];
  uint8_t b1[400];

  memset(b0,0,sizeof(b0));
  memset(b1,0,sizeof(b1));

  ((float*)(b0+2))[0] = 100.f;
  ((float*)(b0+2))[1] = 101.f;
  ((float*)(b0+2))[2] = 102.f;
  ((float*)(b0+2))[3] = 103.f;
  ((float*)(b0+2))[4] = 104.f;
  ((float*)(b0+2))[5] = 105.f;
  ((float*)(b0+2))[6] = 106.f;

  ((float*)(b1+7))[ 0] = 10.f;
  ((float*)(b1+7))[ 1] = 11.f;
  ((float*)(b1+7))[ 2] = 12.f;
  ((float*)(b1+7))[ 3] = 13.f;
  ((float*)(b1+7))[ 4] =  0.f;
  ((float*)(b1+7))[ 5] = 14.f;
  ((float*)(b1+7))[ 6] = 15.f;
  ((float*)(b1+7))[ 7] = 16.f;
  ((float*)(b1+7))[ 8] = 17.f;
  ((float*)(b1+7))[ 9] =  0.f;
  ((float*)(b1+7))[10] = 18.f;
  ((float*)(b1+7))[11] = 19.f;
  ((float*)(b1+7))[12] = 20.f;
  ((float*)(b1+7))[13] = 21.f;

  BufferID vbo0 = cpu_createBuffer(&gpu);
  cpu_bufferData(&gpu,vbo0,sizeof(b0),b0);

  BufferID vbo1 = cpu_createBuffer(&gpu);
  cpu_bufferData(&gpu,vbo1,sizeof(b1),b1);


  auto vao = cpu_createVertexPuller(&gpu);

  cpu_setVertexPuller(&gpu,vao,5,ATTRIBUTE_VEC3,sizeof(float)*2,2,vbo0);
  cpu_enableVertexPullerHead(&gpu,vao,5);

  cpu_setVertexPuller(&gpu,vao,3,ATTRIBUTE_VEC4,sizeof(float)*5,7,vbo1);
  cpu_enableVertexPullerHead(&gpu,vao,3);

  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderVert,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  inVertices.clear();
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,3);
  cpu_finish(&gpu);
  cpu_freeGPU(&gpu);

  REQUIRE(inVertices.size() == 3);
  REQUIRE(*(float*)(inVertices[0].attributes[5].data+sizeof(float)*0) == 100.f);
  REQUIRE(*(float*)(inVertices[0].attributes[5].data+sizeof(float)*1) == 101.f);
  REQUIRE(*(float*)(inVertices[0].attributes[5].data+sizeof(float)*2) == 102.f);
  REQUIRE(*(float*)(inVertices[0].attributes[3].data+sizeof(float)*0) ==  10.f);
  REQUIRE(*(float*)(inVertices[0].attributes[3].data+sizeof(float)*1) ==  11.f);
  REQUIRE(*(float*)(inVertices[0].attributes[3].data+sizeof(float)*2) ==  12.f);
  REQUIRE(*(float*)(inVertices[0].attributes[3].data+sizeof(float)*3) ==  13.f);

  REQUIRE(*(float*)(inVertices[1].attributes[5].data+sizeof(float)*0) == 102.f);
  REQUIRE(*(float*)(inVertices[1].attributes[5].data+sizeof(float)*1) == 103.f);
  REQUIRE(*(float*)(inVertices[1].attributes[5].data+sizeof(float)*2) == 104.f);
  REQUIRE(*(float*)(inVertices[1].attributes[3].data+sizeof(float)*0) ==  14.f);
  REQUIRE(*(float*)(inVertices[1].attributes[3].data+sizeof(float)*1) ==  15.f);
  REQUIRE(*(float*)(inVertices[1].attributes[3].data+sizeof(float)*2) ==  16.f);
  REQUIRE(*(float*)(inVertices[1].attributes[3].data+sizeof(float)*3) ==  17.f);

  REQUIRE(*(float*)(inVertices[2].attributes[5].data+sizeof(float)*0) == 104.f);
  REQUIRE(*(float*)(inVertices[2].attributes[5].data+sizeof(float)*1) == 105.f);
  REQUIRE(*(float*)(inVertices[2].attributes[5].data+sizeof(float)*2) == 106.f);
  REQUIRE(*(float*)(inVertices[2].attributes[3].data+sizeof(float)*0) ==  18.f);
  REQUIRE(*(float*)(inVertices[2].attributes[3].data+sizeof(float)*1) ==  19.f);
  REQUIRE(*(float*)(inVertices[2].attributes[3].data+sizeof(float)*2) ==  20.f);
  REQUIRE(*(float*)(inVertices[2].attributes[3].data+sizeof(float)*3) ==  21.f);

}

SCENARIO("vertex shader should receive correct attributes when using offset and stride and indexing"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  std::vector<float> vert = {0.f,1.f,2.f,3.f};
  BufferID vbo = cpu_createBuffer(&gpu);
  cpu_bufferData(&gpu,vbo,vert.size()*sizeof(decltype(vert)::value_type),vert.data());

  std::vector<uint16_t> indices = {0,1,2,2,1,3};
  BufferID ebo = cpu_createBuffer(&gpu);
  cpu_bufferData(&gpu,ebo,indices.size()*sizeof(decltype(indices)::value_type),indices.data());

  auto vao = cpu_createVertexPuller(&gpu);
  cpu_setVertexPuller(&gpu,vao,0,ATTRIBUTE_FLOAT,sizeof(float),0,vbo);
  cpu_enableVertexPullerHead(&gpu,vao,0);
  cpu_setVertexPullerIndexing(&gpu,vao,UINT16,ebo);

  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderVert,fragmentShaderEmpty);
  cpu_bindVertexPuller(&gpu,vao);

  inVertices.clear();
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,(uint32_t)indices.size());
  cpu_finish(&gpu);
  cpu_freeGPU(&gpu);

  REQUIRE(inVertices.size() == 6);
  REQUIRE(*(float*)inVertices[0].attributes[0].data == 0.f);
  REQUIRE(*(float*)inVertices[1].attributes[0].data == 1.f);
  REQUIRE(*(float*)inVertices[2].attributes[0].data == 2.f);
  REQUIRE(*(float*)inVertices[3].attributes[0].data == 2.f);
  REQUIRE(*(float*)inVertices[4].attributes[0].data == 1.f);
  REQUIRE(*(float*)inVertices[5].attributes[0].data == 3.f);

}

void vertexShaderTri(GPUVertexShaderData*const v){
  if(v->inVertex.gl_VertexID == 0)init_Vec4(&v->outVertex.gl_Position,-1,-1,0,1);
  if(v->inVertex.gl_VertexID == 1)init_Vec4(&v->outVertex.gl_Position,+1,-1,0,1);
  if(v->inVertex.gl_VertexID == 2)init_Vec4(&v->outVertex.gl_Position,-1,+1,0,1);
}

size_t fragmentShaderInvocationCounter = 0;
void fragmentShaderCounter(GPUFragmentShaderData*const f){
  fragmentShaderInvocationCounter++;
  init_Vec4(&f->outFragment.gl_FragColor,0,0,0,0);
}

SCENARIO("fragment shader should be executed roughly pixelCount/2 when rasterizing triangle that covers half of the screen"){
  GPU gpu;
  uint32_t w=100;
  uint32_t h=100;
  cpu_initGPU(&gpu,w,h);
  auto vao = cpu_createVertexPuller(&gpu);
  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderTri,fragmentShaderCounter);
  cpu_bindVertexPuller(&gpu,vao);

  fragmentShaderInvocationCounter=0;
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,3);
  cpu_finish(&gpu);
  cpu_freeGPU(&gpu);


  uint32_t expectedCount = w*h/2;
  uint32_t err = w;
  if(expectedCount < fragmentShaderInvocationCounter)
    REQUIRE(fragmentShaderInvocationCounter <= expectedCount + err);
  else
    REQUIRE(fragmentShaderInvocationCounter >= expectedCount - err);

}

void fragmentShaderUnif(GPUFragmentShaderData*const f){
  uniforms = f->uniforms;
  zero_Vec4(&f->outFragment.gl_FragColor);
}

SCENARIO("fragment shader should receive correct uniform variables from active shader program"){
  GPU gpu;
  uint32_t w=100;
  uint32_t h=100;
  cpu_initGPU(&gpu,w,h);
  auto vao = cpu_createVertexPuller(&gpu);
  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderTri,fragmentShaderUnif);
  cpu_bindVertexPuller(&gpu,vao);

  fragmentShaderInvocationCounter=0;
  uniforms = NULL;
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,3);
  cpu_finish(&gpu);

  auto ptr = &gpu_getActiveProgram(&gpu)->uniforms;
  cpu_freeGPU(&gpu);

  REQUIRE(uniforms == ptr);
}

void vertexShaderPD(GPUVertexShaderData*const v){
  float f0 = 0.1f;
  float f1 = 0.3f;
  float f2 = 4.f;
  if(v->inVertex.gl_VertexID == 0)init_Vec4(&v->outVertex.gl_Position,-1*f0,-1*f0,0,1*f0);
  if(v->inVertex.gl_VertexID == 1)init_Vec4(&v->outVertex.gl_Position,+1*f1,-1*f1,0,1*f1);
  if(v->inVertex.gl_VertexID == 2)init_Vec4(&v->outVertex.gl_Position,-1*f2,+1*f2,0,1*f2);
}

SCENARIO("perspective division should be performed"){
  GPU gpu;
  uint32_t w=100;
  uint32_t h=100;
  cpu_initGPU(&gpu,w,h);
  auto vao = cpu_createVertexPuller(&gpu);
  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderPD,fragmentShaderCounter);
  cpu_bindVertexPuller(&gpu,vao);

  cpu_useProgram(&gpu,prg);
  fragmentShaderInvocationCounter = 0;
  cpu_drawTriangles(&gpu,3);
  cpu_finish(&gpu);
  cpu_freeGPU(&gpu);

  uint32_t expectedCount = w*h/2;
  uint32_t err = w;
  if(expectedCount < fragmentShaderInvocationCounter)
    REQUIRE(fragmentShaderInvocationCounter <= expectedCount + err);
  else
    REQUIRE(fragmentShaderInvocationCounter >= expectedCount - err);

}

std::vector<Vec4>triVertices;

void initTriVertices(){
  triVertices.clear();
  Vec4 a0;init_Vec4(&a0,-1,-1,0,1);triVertices.push_back(a0);
  Vec4 a1;init_Vec4(&a1,+1,-1,0,1);triVertices.push_back(a1);
  Vec4 a2;init_Vec4(&a2,-1,+1,0,1);triVertices.push_back(a2);
}

void vertexShaderInterp(GPUVertexShaderData*const v){
  if(v->inVertex.gl_VertexID == 0){
    copy_Vec4(&v->outVertex.gl_Position,&triVertices.at(0));
    init_Vec3((Vec3*)v->outVertex.attributes[0].data,1,0,0);
  }
  if(v->inVertex.gl_VertexID == 1){
    copy_Vec4(&v->outVertex.gl_Position,&triVertices.at(1));
    init_Vec3((Vec3*)v->outVertex.attributes[0].data,0,1,0);
  }
  if(v->inVertex.gl_VertexID == 2){
    copy_Vec4(&v->outVertex.gl_Position,&triVertices.at(2));
    init_Vec3((Vec3*)v->outVertex.attributes[0].data,0,0,1);
  }
}


std::vector<Vec2>samplingLocations;

size_t whichSample(Vec4&coord){
  //std::cerr << "x: " << coord.data[0] << " y: " << coord.data[1] << std::endl;
  for(size_t i=0;i<samplingLocations.size();++i){
    if(
      equalFloats(coord.data[0] , samplingLocations.at(i).data[0] + 0.5f) &&
      equalFloats(coord.data[1] , samplingLocations.at(i).data[1] + 0.5f)
    )return i;
  }
  return samplingLocations.size()+1;
}

std::vector<size_t>hitSamples;
std::vector<Vec3>sampleColors;
std::vector<Vec4>fragCoords;
void fragmentShaderInterp(GPUFragmentShaderData*const f){
  size_t id = whichSample(f->inFragment.gl_FragCoord);
  if(id < samplingLocations.size()){
    hitSamples.push_back(id);
    Vec3 col;
    copy_Vec3(&col,(Vec3*)f->inFragment.attributes[0].data);
    sampleColors.push_back(col);
    fragCoords.push_back(f->inFragment.gl_FragCoord);
  }
  zero_Vec4(&f->outFragment.gl_FragColor);
}

SCENARIO("rasterization should create fragments that are inside triangle (gl_FragCoord.xy should contain correct screen space position)"){
  GPU gpu;
  uint32_t w=100;
  uint32_t h=100;
  cpu_initGPU(&gpu,w,h);
  auto vao = cpu_createVertexPuller(&gpu);
  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderInterp,fragmentShaderInterp);
  cpu_bindVertexPuller(&gpu,vao);

  
  samplingLocations.clear();
  samplingLocations.push_back({0 ,0 });
  samplingLocations.push_back({99,0 });
  samplingLocations.push_back({0 ,98});
  samplingLocations.push_back({90,5 });
  samplingLocations.push_back({33,33});
  samplingLocations.push_back({33,40});
  samplingLocations.push_back({40,33});
  hitSamples.clear();
  sampleColors.clear();
  fragCoords.clear();
  initTriVertices();
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,3);
  cpu_finish(&gpu);
  cpu_freeGPU(&gpu);

  REQUIRE(hitSamples.size() == samplingLocations.size());

}

SCENARIO("rasterization should not create fragments that are outside triangle (gl_FragCoord.xy should contain correct screen space position)"){
  GPU gpu;
  uint32_t w=100;
  uint32_t h=100;
  cpu_initGPU(&gpu,w,h);
  auto vao = cpu_createVertexPuller(&gpu);
  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderInterp,fragmentShaderInterp);
  cpu_bindVertexPuller(&gpu,vao);

  samplingLocations.clear();
  samplingLocations.push_back({99,1 });
  samplingLocations.push_back({1 ,99});
  samplingLocations.push_back({51,51});
  samplingLocations.push_back({60,60});
  samplingLocations.push_back({60,70});
  samplingLocations.push_back({70,60});
  samplingLocations.push_back({99,99});
  samplingLocations.push_back({10,10});

  hitSamples.clear();
  sampleColors.clear();
  fragCoords.clear();
  initTriVertices();
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,3);
  cpu_finish(&gpu);
  cpu_freeGPU(&gpu);

  REQUIRE(hitSamples.size() == 1);

}

SCENARIO("rasterization should interpolate vertex attributes using barycentric coordinates"){
  GPU gpu;
  uint32_t w=100;
  uint32_t h=100;
  cpu_initGPU(&gpu,w,h);
  auto vao = cpu_createVertexPuller(&gpu);
  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderInterp,fragmentShaderInterp);
  cpu_setVS2FSType(&gpu,prg,0,ATTRIBUTE_VEC3);
  cpu_bindVertexPuller(&gpu,vao);

  Vec2 fragCoord = {30,20};

  samplingLocations.clear();
  samplingLocations.push_back(fragCoord);
  hitSamples.clear();
  sampleColors.clear();
  fragCoords.clear();
  initTriVertices();
  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,3);
  cpu_finish(&gpu);
  cpu_freeGPU(&gpu);

  REQUIRE(hitSamples.size() == samplingLocations.size());
  float area = w*h/2.f;
  float l2 = ((fragCoord.data[1]+.5f)*w/2.f) / area;
  float l1 = ((fragCoord.data[0]+.5f)*h/2.f) / area;
  float l0 = 1.f - l1 - l2;
  REQUIRE(sampleColors.at(0).data[0] == 10);
  REQUIRE(equalFloats(sampleColors.at(0).data[0],l0));
  REQUIRE(equalFloats(sampleColors.at(0).data[1],l1));
  REQUIRE(equalFloats(sampleColors.at(0).data[2],l2));

}

SCENARIO("rasterization should interpolate vertex attributes using barycentric coordinates with perspective correction and interpolate correct fragment depth"){
  GPU gpu;
  uint32_t w=100;
  uint32_t h=100;
  cpu_initGPU(&gpu,w,h);
  auto vao = cpu_createVertexPuller(&gpu);
  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderInterp,fragmentShaderInterp);
  cpu_setVS2FSType(&gpu,prg,0,ATTRIBUTE_VEC3);
  cpu_bindVertexPuller(&gpu,vao);

  Vec2 fragCoord = {30,20};

  samplingLocations.clear();
  samplingLocations.push_back(fragCoord);
  hitSamples.clear();
  sampleColors.clear();
  fragCoords.clear();

  float hc[3] = {1,2,.5};
  float zz[3] = {.9f,.4f,.8f};
  triVertices.clear();
  triVertices.push_back({-hc[0],-hc[0],zz[0],hc[0]});
  triVertices.push_back({+hc[1],-hc[1],zz[1],hc[1]});
  triVertices.push_back({-hc[2],+hc[2],zz[2],hc[2]});

  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,3);
  cpu_finish(&gpu);
  cpu_freeGPU(&gpu);

  REQUIRE(hitSamples.size() == samplingLocations.size());
  float area = w*h/2.f;
  float l2 = ((fragCoord.data[1]+.5f)*w/2.f) / area;
  float l1 = ((fragCoord.data[0]+.5f)*h/2.f) / area;
  float l0 = 1.f - l1 - l2;
  float divisor = l0/hc[0] + l1/hc[1] + l2/hc[2];
  REQUIRE(sampleColors.at(0).data[0] == 10/hc[0]/divisor);
  REQUIRE(equalFloats(sampleColors.at(0).data[0],l0/hc[0]/divisor));
  REQUIRE(equalFloats(sampleColors.at(0).data[1],l1/hc[1]/divisor));
  REQUIRE(equalFloats(sampleColors.at(0).data[2],l2/hc[2]/divisor));

  REQUIRE(equalFloats(fragCoords.at(0).data[2],
        (zz[0]/hc[0]*l0/hc[0] + zz[1]/hc[1]*l1/hc[1] + zz[2]/hc[2]*l2/hc[2])/divisor));
}

void fragmentShaderWhite(GPUFragmentShaderData*const f){
  init_Vec4(&f->outFragment.gl_FragColor,1,1,1,1);
}

SCENARIO("per fragment operations should eliminate fragments that do not pass depth test"){
  GPU gpu;
  uint32_t w=100;
  uint32_t h=100;
  cpu_initGPU(&gpu,w,h);
  auto vao = cpu_createVertexPuller(&gpu);
  auto prg = cpu_createProgram(&gpu);
  cpu_attachShaders(&gpu,prg,vertexShaderTri,fragmentShaderWhite);
  cpu_bindVertexPuller(&gpu,vao);

  //clear framebuffer
  for(uint32_t y=0;y<h;++y)
    for(uint32_t x=0;x<w;++x){
      uint32_t p = y*w+x;
      init_Vec4(gpu.framebuffer.color+p,0,0,0,0);
      gpu.framebuffer.depth[p] = +1.f;
    }

  //set one pixel depth to near plane
  gpu.framebuffer.depth[10*w+10] = -1.f;

  cpu_useProgram(&gpu,prg);
  cpu_drawTriangles(&gpu,3);
  cpu_finish(&gpu);

  std::vector<Vec4>color;
  std::vector<float>depth;

  for(uint32_t i=0;i<w*h;++i){
    color.push_back(gpu.framebuffer.color[i]);
    depth.push_back(gpu.framebuffer.depth[i]);
  }

  cpu_freeGPU(&gpu);


  //point outside of triangle
  REQUIRE(equalFloats(depth[80*w+80],+1.f));
  REQUIRE(equalFloats(depth[80*w+80],+1.f));
  REQUIRE(equalFloats(depth[80*w+80],+1.f));

  REQUIRE(equalFloats(color[80*w+80].data[0],0.f));
  REQUIRE(equalFloats(color[80*w+80].data[1],0.f));
  REQUIRE(equalFloats(color[80*w+80].data[2],0.f));

  //point inside of triangle
  REQUIRE(equalFloats(depth[20*w+20],0.f));
  REQUIRE(equalFloats(depth[20*w+20],0.f));
  REQUIRE(equalFloats(depth[20*w+20],0.f));

  REQUIRE(equalFloats(color[20*w+20].data[0],1.f));
  REQUIRE(equalFloats(color[20*w+20].data[1],1.f));
  REQUIRE(equalFloats(color[20*w+20].data[2],1.f));

  //point inside of triangle (with modified depth)
  REQUIRE(equalFloats(depth[10*w+10],-1.f));
  REQUIRE(equalFloats(depth[10*w+10],-1.f));
  REQUIRE(equalFloats(depth[10*w+10],-1.f));

  REQUIRE(equalFloats(color[10*w+10].data[0],0.f));
  REQUIRE(equalFloats(color[10*w+10].data[1],0.f));
  REQUIRE(equalFloats(color[10*w+10].data[2],0.f));

}

SCENARIO("vertex shader should forward world space position and normal"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUVertexShaderData vd;
  memset(vd.outVertex.attributes[0].data,0,MAX_ATTRIBUTE_SIZE);
  memset(vd.outVertex.attributes[1].data,0,MAX_ATTRIBUTE_SIZE);
  vd.uniforms = &u;

  init_Vec3((Vec3*)vd.inVertex.attributes[0].data,1.f,2.f,3.f);
  init_Vec3((Vec3*)vd.inVertex.attributes[1].data,0,0,1.f);
  translate_Mat4((Mat4*)u.uniform[0].data,10.f,20.f,30.f);
  translate_Mat4((Mat4*)u.uniform[1].data,30.f,40.f,50.f);

  phong_VS(&vd);

  Vec3* a0 = (Vec3*)vd.outVertex.attributes[0].data;
  Vec3* a1 = (Vec3*)vd.outVertex.attributes[1].data;
  REQUIRE(equalFloats(a0->data[0] , 1.f));
  REQUIRE(equalFloats(a0->data[1] , 2.f));
  REQUIRE(equalFloats(a0->data[2] , 3.f));

  REQUIRE(equalFloats(a1->data[0] , 0.f));
  REQUIRE(equalFloats(a1->data[1] , 0.f));
  REQUIRE(equalFloats(a1->data[2] , 1.f));
}

SCENARIO("vertex shader should compute correct gl_Position according to transformation matrices"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUVertexShaderData vd;
  zero_Vec4(&vd.outVertex.gl_Position);
  vd.uniforms = &u;

  init_Vec3((Vec3*)vd.inVertex.attributes[0].data,1.f,2.f,3.f);
  translate_Mat4((Mat4*)u.uniform[0].data,1.f,3.f,4.f);
  scale_Mat4((Mat4*)u.uniform[1].data,1.f,2.f,1.f);

  phong_VS(&vd);

  REQUIRE(equalFloats (vd.outVertex.gl_Position.data[0] ,  2.f));
  REQUIRE(equalFloats (vd.outVertex.gl_Position.data[1] , 10.f));
  REQUIRE(equalFloats (vd.outVertex.gl_Position.data[2] ,  7.f));
  REQUIRE(equalFloats (vd.outVertex.gl_Position.data[3] ,  1.f));
}


SCENARIO("fragment shader should compute correct color for vertical normals"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUFragmentShaderData fd;
  fd.uniforms = &u;
  init_Vec4(&fd.outFragment.gl_FragColor,.1f,.2f,.3f,.4f);

  init_Vec3((Vec3*)fd.inFragment.attributes[0].data,0,0,0);
  init_Vec3((Vec3*)fd.inFragment.attributes[1].data,0,1,0);
  init_Vec3((Vec3*)u.uniform[2].data,0,1,0);
  init_Vec3((Vec3*)u.uniform[3].data,1,0,0);

  phong_FS(&fd);

  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[0] , 1.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[1] , 1.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[2] , 1.f));
}

SCENARIO("fragment shader should compute correct color for horizontal normals"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUFragmentShaderData fd;
  fd.uniforms = &u;
  init_Vec4(&fd.outFragment.gl_FragColor,.1f,.2f,.3f,.4f);
  init_Vec3((Vec3*)fd.inFragment.attributes[0].data,0,0,0);
  init_Vec3((Vec3*)fd.inFragment.attributes[1].data,1,0,0);
  init_Vec3((Vec3*)u.uniform[2].data,1,0,0);
  init_Vec3((Vec3*)u.uniform[3].data,0,0,1);

  phong_FS(&fd);

  REQUIRE(equalFloats (fd.outFragment.gl_FragColor.data[0] , 0.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[1] , 1.f));
  REQUIRE(equalFloats (fd.outFragment.gl_FragColor.data[2] , 0.f));
}

SCENARIO("fragment shader should compute correct color for backfacing triangles"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUFragmentShaderData fd;
  fd.uniforms = &u;
  init_Vec4(&fd.outFragment.gl_FragColor,.1f,.2f,.3f,.4f);
  init_Vec3((Vec3*)fd.inFragment.attributes[0].data,0,0,0);
  init_Vec3((Vec3*)fd.inFragment.attributes[1].data,1,0,0);
  init_Vec3((Vec3*)u.uniform[2].data,0,1,0);
  init_Vec3((Vec3*)u.uniform[3].data,1,0,0);


  phong_FS(&fd);

  REQUIRE(equalFloats(fd.outFragment.gl_FragColor.data[0] , 0.f));
  REQUIRE(equalFloats(fd.outFragment.gl_FragColor.data[1] , 0.f));
  REQUIRE(equalFloats(fd.outFragment.gl_FragColor.data[2] , 0.f));
}

SCENARIO("fragment shader should compute correct color for specular reflections"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUFragmentShaderData fd;
  fd.uniforms = &u;

  init_Vec3((Vec3*)fd.inFragment.attributes[0].data,0,0,0);
  init_Vec3((Vec3*)fd.inFragment.attributes[1].data,1,0,0);
  init_Vec3((Vec3*)u.uniform[2].data,1,0,0);
  init_Vec3((Vec3*)u.uniform[3].data,1,0,0);
  init_Vec4(&fd.outFragment.gl_FragColor,.1f,.2f,.3f,.4f);

  phong_FS(&fd);

  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[0] , 1.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[1] , 1.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[2] , 1.f));
}

SCENARIO("fragment shader should not compute specular reflection for backfacing triangles"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUFragmentShaderData fd;
  fd.uniforms = &u;

  init_Vec3((Vec3*)fd.inFragment.attributes[0].data,0,0,0);
  init_Vec3((Vec3*)fd.inFragment.attributes[1].data,1,0,0);
  init_Vec3((Vec3*)u.uniform[2].data,1,0,0);
  init_Vec3((Vec3*)u.uniform[3].data,-1,0,0);
  init_Vec4(&fd.outFragment.gl_FragColor,.1f,.2f,.3f,.4f);

  phong_FS(&fd);

  REQUIRE(equalFloats (fd.outFragment.gl_FragColor.data[0] , 0.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[1] , 1.f));
  REQUIRE(equalFloats (fd.outFragment.gl_FragColor.data[2] , 0.f));
}

SCENARIO("fragment shader should not compute specular reflection for black triangles"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUFragmentShaderData fd;
  fd.uniforms = &u;

  init_Vec3((Vec3*)fd.inFragment.attributes[0].data,0,0,0);
  init_Vec3((Vec3*)fd.inFragment.attributes[1].data,0,-1,0);
  init_Vec3((Vec3*)u.uniform[2].data,0,1,0);
  init_Vec3((Vec3*)u.uniform[3].data,-1,0,0);
  init_Vec4(&fd.outFragment.gl_FragColor,.1f,.2f,.3f,.4f);

  phong_FS(&fd);

  REQUIRE(equalFloats(fd.outFragment.gl_FragColor.data[0] , 0.f));
  REQUIRE(equalFloats(fd.outFragment.gl_FragColor.data[1] , 0.f));
  REQUIRE(equalFloats(fd.outFragment.gl_FragColor.data[2] , 0.f));
}

SCENARIO("fragment shader should normalize normals"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUFragmentShaderData fd;
  fd.uniforms = &u;

  init_Vec3((Vec3*)fd.inFragment.attributes[0].data,0,0,0);
  init_Vec3((Vec3*)fd.inFragment.attributes[1].data,.1f,0,0);
  init_Vec3((Vec3*)u.uniform[2].data,1,0,0);
  init_Vec3((Vec3*)u.uniform[3].data,0,1,0);
  init_Vec4(&fd.outFragment.gl_FragColor,.1f,.2f,.3f,.4f);

  phong_FS(&fd);

  REQUIRE(equalFloats (fd.outFragment.gl_FragColor.data[0] , 0.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[1] , 1.f));
  REQUIRE(equalFloats (fd.outFragment.gl_FragColor.data[2] , 0.f));
}


SCENARIO("fragment shader lighting should not depend on the distance to the viewer"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUFragmentShaderData fd;
  fd.uniforms = &u;

  init_Vec3((Vec3*)fd.inFragment.attributes[0].data,0,0,0);
  init_Vec3((Vec3*)fd.inFragment.attributes[1].data,1,0,0);
  init_Vec3((Vec3*)u.uniform[2].data,1.f,0.f,0.f);
  init_Vec3((Vec3*)u.uniform[3].data,.1f,0.f,0.f);
  init_Vec4(&fd.outFragment.gl_FragColor,.1f,.2f,.3f,.4f);

  phong_FS(&fd);

  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[0] , 1.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[1] , 1.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[2] , 1.f));
}

SCENARIO("fragment shader lighting should not depend on the distance to the light"){
  GPUUniforms u;
  gpu_initUniforms(&u);
  GPUFragmentShaderData fd;
  fd.uniforms = &u;

  init_Vec3((Vec3*)fd.inFragment.attributes[0].data,0,0,0);
  init_Vec3((Vec3*)fd.inFragment.attributes[1].data,1,0,0);
  init_Vec3((Vec3*)u.uniform[2].data,.1f,0.f,0.f);
  init_Vec3((Vec3*)u.uniform[3].data,1.f,0.f,0.f);
  init_Vec4(&fd.outFragment.gl_FragColor,.1f,.2f,.3f,.4f);

  phong_FS(&fd);

  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[0] , 1.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[1] , 1.f));
  REQUIRE(greaterFloat(fd.outFragment.gl_FragColor.data[2] , 1.f));
}


SCENARIO("phong method should render correct image"){
  GPU gpu;
  int w=500;
  int h=500;
  cpu_initGPU(&gpu,w,h);
  phong_onInit(&gpu);

  initGlobals();

  phong_onDraw(&gpu);
  cpu_finish(&gpu);

  int width = gpu.framebuffer.width;
  int height = gpu.framebuffer.height;
  std::vector<Vec4>colors;
  for(int i=0;i<width*height;++i)
    colors.push_back(gpu.framebuffer.color[i]);

  phong_onExit(&gpu);
  cpu_freeGPU(&gpu);

  SDL_Surface* groundTruth = SDL_LoadBMP(groundTruthFile);

  if (groundTruth == nullptr) {
    std::cerr << "ERROR: reference image: \"" << groundTruthFile
              << "\" is corrupt!" << std::endl;
    REQUIRE(groundTruth != nullptr);
  }

  if (width == groundTruth->w && height == groundTruth->h) {
    float meanSquareError = 0;
    for (int32_t y = 0; y < height; ++y)
      for (int32_t x = 0; x < width; ++x){
        for (int32_t c = 0; c < 3; ++c) {
          float col = colors[(height-y-1)*width+x].data[c];
          if(col>1.f)col=1.f;
          if(col<0.f)col=0.f;
          uint8_t ucol = (uint8_t)(col*255);
          uint8_t gcol = ((uint8_t*)groundTruth->pixels)[y * groundTruth->pitch + x*groundTruth->format->BytesPerPixel + c];
          float diff = fabsf((float)ucol - (float)gcol);
          diff *= diff;
          meanSquareError += diff;
        }
      }

    meanSquareError /= (float)(width * height * 3);
    SDL_FreeSurface(groundTruth);

    REQUIRE(meanSquareError < 40.f);
  }

}

// */
