#pragma once

#include <student/gpu.h>
#include <student/window.h>
#include <student/method.h>

/**
 * @brief This struct represent 3D application
 */
struct AppData{
  GPU      gpu        ;///< GPU
  Window   window     ;///< window
  Vector   methods    ;///< all rendering methods
  uint32_t methodId   ;///< current method
  uint32_t oldMethodId;///< old method
  float    cameraAngleX;///< x angle of camera
  float    cameraAngleY;///< y angle of camera
  float    cameraDistance;///< camera distance from center
};

typedef struct AppData AppData;///< shortcut

#ifdef __cplusplus
extern "C" {
#endif

void app_onDraw(void*d);
void app_init(AppData*const app,int32_t width,int32_t height,char const*name);
void app_start(AppData*const app);
void app_free(AppData*const app);

#ifdef __cplusplus
}
#endif
