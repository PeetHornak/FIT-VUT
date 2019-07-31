#include<catch.hpp>
#include<iostream>

#include<student/gpu.h>
#include<student/cpu.h>

SCENARIO("empty gpu allocation test"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  REQUIRE(gpu.activeProgram == EMPTY_ID);
  REQUIRE(gpu.activeVertexPuller == EMPTY_ID);

  cpu_freeGPU(&gpu);
}

SCENARIO("gpu buffer test"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  float d[2] = {0.f,1.f};
  auto id = cpu_createBuffer(&gpu);
  cpu_bufferData(&gpu,id,sizeof(d),d);
  REQUIRE(id != EMPTY_ID);
  cpu_deleteBuffer(&gpu,id);
  auto id2 = cpu_createBuffer(&gpu);
  REQUIRE(id2 == id);
  cpu_deleteBuffer(&gpu,id2);

  cpu_freeGPU(&gpu);
}

void vs(GPUVertexShaderData*const d){(void)d;}
void fs(GPUFragmentShaderData*const d){(void)d;}

SCENARIO("gpu program test"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);
  auto id = cpu_createProgram(&gpu);
  REQUIRE(id != EMPTY_ID);
  cpu_deleteProgram(&gpu,id);
  auto id2 = cpu_createProgram(&gpu);
  REQUIRE(id2 == id);
  cpu_attachShaders(&gpu,id2,vs,fs);
  cpu_deleteProgram(&gpu,id2);


  cpu_freeGPU(&gpu);
}

SCENARIO("gpu vertex puller test"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  auto id = cpu_createVertexPuller(&gpu);
  REQUIRE(id != EMPTY_ID);
  cpu_deleteVertexPuller(&gpu,id);
  auto id2 = cpu_createProgram(&gpu);
  REQUIRE(id2 == id);
  cpu_deleteVertexPuller(&gpu,id2);

  cpu_freeGPU(&gpu);
}

SCENARIO("cpu_freeGPU"){
  GPU gpu;
  cpu_initGPU(&gpu,100,100);

  float d[2] = {0.f,1.f};
  auto id = cpu_createBuffer(&gpu);
  cpu_bufferData(&gpu,id,sizeof(d),d);

  cpu_freeGPU(&gpu);
}

