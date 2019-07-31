#include <assert.h>
#include <student/application.h>
#include <student/cpu.h>
#include <student/emptyMethod.h>
#include <student/pointMethod.h>
#include <student/pointCircleMethod.h>
#include <student/pointSquareMethod.h>
#include <student/pointBoxMethod.h>
#include <student/phongMethod.h>
#include <student/triangleMethod.h>
#include <student/triangle3DMethod.h>
#include <student/globals.h>
#include <student/camera.h>

void app_onDraw(void*a){
  AppData*app = (AppData*)a;
  Method*m = (Method*)vector_at(&app->methods,app->methodId);
  if(app->oldMethodId != app->methodId){
    Method*old = (Method*)vector_at(&app->methods,app->oldMethodId);
    call(&old->onExit);
    call(&m->onInit);
    SDL_SetWindowTitle(app->window.window,m->name);
    app->oldMethodId = app->methodId;
  }
  call(&m->onDraw);
  cpu_swapBuffers(app->window.surface,&app->gpu);
}

void app_onExit(void*a){
  AppData*app = (AppData*)a;
  Method*m = (Method*)vector_at(&app->methods,app->methodId);
  call(&m->onExit);
}

void app_onKeyDown(SDL_Event const*const e,void*a){
  AppData*app = (AppData*)a;
  if (e->key.keysym.sym == SDLK_n){
    uint32_t nofMethods = vector_size(&app->methods);
    app->methodId++;
    if(app->methodId == nofMethods)app->methodId=0;
  }
  if (e->key.keysym.sym == SDLK_p){
    uint32_t nofMethods = vector_size(&app->methods);
    if(app->methodId > 0)app->methodId--;
    else app->methodId = nofMethods-1;
  }
  if (e->key.keysym.sym == SDLK_ESCAPE){
    app->window.running = 0;
  }
}

void app_onMouseMotion(SDL_Event const*const e,void*a){
  AppData*app = (AppData*)a;
  float yrel = (float)e->motion.yrel;
  float xrel = (float)e->motion.xrel;

  float const cameraSensitivity = 0.01f;
  float const cameraMinDistance = 1.f;
  float const cameraMaxDistance = 100.f;
  float const cameraZoomSpeed   = 1.f;
  int changed = 0;

  if(e->motion.state & SDL_BUTTON_LMASK){
    app->cameraAngleX += yrel * cameraSensitivity;
    app->cameraAngleY += xrel * cameraSensitivity;
    if (app->cameraAngleX < -MY_PI / 2.f) app->cameraAngleX = -MY_PI / 2.f;
    if (app->cameraAngleX > +MY_PI / 2.f) app->cameraAngleX = +MY_PI / 2.f;
    changed                                                 = 1;
  }
  if(e->motion.state & SDL_BUTTON_RMASK){
    app->cameraDistance += yrel * cameraZoomSpeed;
    if (app->cameraDistance < cameraMinDistance) app->cameraDistance = cameraMinDistance;
    if (app->cameraDistance > cameraMaxDistance) app->cameraDistance = cameraMaxDistance;
    changed                                                          = 1;
  }
  if (changed) {
    cpu_computeOrbitCamera(&viewMatrix, app->cameraAngleX, app->cameraAngleY,
                           app->cameraDistance);
    cpu_getCameraPositionFromViewMatrix(&cameraPosition, &viewMatrix);
  }
}

void pushMethod(AppData*const app,Callback onInit,Callback onDraw,Callback onExit,char const*n){
  Method m;
  method_init(&m);
  m.onInit.callback = onInit;
  m.onInit.data     = &app->gpu;
  m.onDraw.callback = onDraw;
  m.onDraw.data     = &app->gpu;
  m.onExit.callback = onExit;
  m.onExit.data     = &app->gpu;
  m.name            = n;
  vector_pushback(&app->methods,&m);
}

/**
 * @brief This function inits application
 * It creates GPU and window.
 *
 * @param app application
 * @param width width of the window
 * @param height height of the window
 * @param name name of the window
 */
void app_init(AppData*const app,int32_t width,int32_t height,char const*name){
  app->cameraAngleX = 0.f;
  app->cameraAngleY = 0.f;
  app->cameraDistance = 3.f;
  float const cameraFovy = MY_PI / 2.f;
  float const cameraFar  = 10000.f;
  float const cameraNear = 0.1f;

  float const aspectRatio = (float)width / (float)height;
  perspective_Mat4(&projectionMatrix, cameraFovy, aspectRatio, cameraNear,
                   cameraFar);
  cpu_computeOrbitCamera(&viewMatrix, app->cameraAngleX, app->cameraAngleY,
                         app->cameraDistance);
  cpu_getCameraPositionFromViewMatrix(&cameraPosition, &viewMatrix);
  init_Vec3(&lightPosition,0.f,100.f,0.f);

  cpu_initGPU(&app->gpu,width,height);
  cpu_enableDebug(&app->gpu);
  window_init(&app->window,width,height,name);

  vector_init(&app->methods,sizeof(Method));

  pushMethod(app,phong_onInit,phong_onDraw,phong_onExit,"phong");
  pushMethod(app,empty_onInit,empty_onDraw,empty_onExit,"empty");
  pushMethod(app,point_onInit,point_onDraw,point_onExit,"point");
  pushMethod(app,pointCircle_onInit,pointCircle_onDraw,pointCircle_onExit,"pointCircle");
  pushMethod(app,pointSquare_onInit,pointSquare_onDraw,pointSquare_onExit,"pointSquare");
  pushMethod(app,pointBox_onInit,pointBox_onDraw,pointBox_onExit,"pointBox");
  pushMethod(app,triangle_onInit,triangle_onDraw,triangle_onExit,"triangle");
  pushMethod(app,triangle3d_onInit,triangle3d_onDraw,triangle3d_onExit,"triangle3d");


  app->methodId = 0;
  app->oldMethodId = 0;

  Method*activeMethod=(Method*)vector_at(&app->methods,app->methodId);
  call(&activeMethod->onInit);
  SDL_SetWindowTitle(app->window.window,activeMethod->name);

  window_setIdleCallback(&app->window,app_onDraw,app);
  window_setCallback(&app->window,SDL_KEYDOWN,app_onKeyDown,app);
  window_setCallback(&app->window,SDL_MOUSEMOTION,app_onMouseMotion,app);
}


/**
 * @brief This function starts application main loop
 *
 * @param app application
 */
void app_start(AppData*const app){
  window_mainloop(&app->window);
}

/**
 * @brief This function frees application
 *
 * @param app application
 */
void app_free(AppData*const app){
  cpu_freeGPU(&app->gpu);
  window_free(&app->window);
}


