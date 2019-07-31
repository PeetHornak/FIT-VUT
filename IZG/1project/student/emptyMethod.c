#include <student/emptyMethod.h>
#include <student/application.h>
#include <student/cpu.h>

void empty_onInit(void*a){
  GPU*gpu = (GPU*)a;
  Vec4 clear;
  init_Vec4(&clear,.1f,.1f,.1f,1.f);
  cpu_clearColor(gpu,clear);
}

void empty_onDraw(void*a){
  GPU*gpu = (GPU*)a;
  cpu_clear(gpu);
}

void empty_onExit(void*a){
  (void)a;
}
