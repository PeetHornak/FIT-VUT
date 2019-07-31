#include <tests/takeScreenShot.h>
#include <student/cpu.h>
#include <student/application.h>
#include <student/phongMethod.h>
#include <student/globals.h>
#include <tests/groundTruth.h>

void takeScreenShot(){
  AppData app;
  app_init(&app,500,500,"");
  initGlobals();
  app_onDraw(&app);

  SDL_Surface* rgb = SDL_ConvertSurfaceFormat(app.window.surface, SDL_PIXELFORMAT_RGB24, 0);
  SDL_SaveBMP(rgb, groundTruthFile);
  SDL_FreeSurface(rgb);

  app_free(&app);
}
