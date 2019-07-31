#pragma once

#include<SDL.h>


/**
 * @brief This type represents event callback (function pointer)
 */
typedef void(*EventCallback)(SDL_Event const*const,void*);

/**
 * @brief This type represents draw/idle callback
 *
 */
typedef void(*IdleCallback)(void*);

/**
 * @brief This struct represents SDL window
 */
struct Window{
  SDL_Window*   window          ;///< window
  SDL_Surface*  surface         ;///< surface
  SDL_Renderer* renderer        ;///< SDL2 renderer
  int32_t       running         ;///< is main loop running
  EventCallback*eventCallback   ;///< event callbacks (hash table would be nicer...)
  void**        callbackData    ;///< data for event callbacks
  IdleCallback  idleCallback    ;///< idle/draw callback
  void*         idleCallbackData;///< idle/draw callback data
};

typedef struct Window Window;///< shortcut

void window_init(Window*const window,int32_t width,int32_t height,char const*name);
void window_free(Window*const window);
void window_mainloop(Window*const window);
void window_setCallback(Window*const window,uint32_t event,EventCallback clb,void*data);
void window_setIdleCallback(Window*const window,IdleCallback clb,void*data);
void window_defaultQuitCallback(SDL_Event const*const event,void*data);
