#pragma once

#include <student/gpu.h>

#ifdef __cplusplus
extern "C" {
#endif

void phong_VS(GPUVertexShaderData*const data);
void phong_FS(GPUFragmentShaderData*const data);
void phong_onInit(void*a);
void phong_onDraw(void*a);
void phong_onExit(void*a);

#ifdef __cplusplus
}
#endif
