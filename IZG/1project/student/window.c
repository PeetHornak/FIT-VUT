#include <assert.h>
#include <student/window.h>
#include <string.h>

void window_defaultQuitCallback(SDL_Event const* const event, void* data)
{
  (void)event;
  Window* window  = (Window*)data;
  window->running = 0;
}

void window_initSDL(Window* const window)
{
  // enable logging
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

  // initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) == 0) return;

  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail: %s\n",
               SDL_GetError());
  exit(1);
}

void window_initWindow(Window* const window,
                       int32_t       width,
                       int32_t       height,
                       char const*   name)
{
  // create window
  window->window =
      SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_SHOWN);
  if (window) return;
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow fail: %s\n",
               SDL_GetError());
  exit(1);
}

void window_initSurface(Window* const window)
{
  // create surface
  window->surface = SDL_GetWindowSurface(window->window);
  if (window->surface) return;
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GetWindowSurface fail: %s\n",
               SDL_GetError());
  exit(1);
}

void window_initRenderer(Window* const window)
{
  // create renderer
  window->renderer = SDL_CreateSoftwareRenderer(window->surface);
  if (window->renderer) return;
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateSoftwareRenderer: %s\n",
               SDL_GetError());
  exit(1);
}

void window_initEvents(Window* const window)
{
  window->callbackData = malloc(sizeof(void*)*SDL_LASTEVENT);
  window->eventCallback = malloc(sizeof(EventCallback)*SDL_LASTEVENT);
  memset(window->callbackData, 0, sizeof(void*)*SDL_LASTEVENT);
  memset(window->eventCallback, 0, sizeof(EventCallback)*SDL_LASTEVENT);
  window_setCallback(window, SDL_QUIT, window_defaultQuitCallback, window);
}

/**
 * @brief This function inits window.
 *
 * @param window window
 * @param width width of the window
 * @param height height of the window
 * @param name name of the window
 */
void window_init(Window* const window,
                 int32_t       width,
                 int32_t       height,
                 char const*   name)
{
  assert(window != NULL);

  window_initSDL(window);
  window_initWindow(window, width, height, name);
  window_initSurface(window);
  window_initRenderer(window);
  window_initEvents(window);
}

/**
 * @brief This function frees window
 *
 * @param window window
 */
void window_free(Window* const window)
{
  assert(window != NULL);
  if(window->callbackData)free(window->callbackData);
  window->callbackData = NULL;
  if(window->eventCallback)free(window->eventCallback);
  window->eventCallback = NULL;
  SDL_DestroyRenderer(window->renderer);
  SDL_DestroyWindow(window->window);
  SDL_Quit();
}

void window_takeCareOfEvent(Window* const window, SDL_Event const* const event)
{
  EventCallback callback = window->eventCallback[event->type];
  if (callback) callback(event, window->callbackData[event->type]);
}

/**
 * @brief This is the main loop of the window
 *
 * @param window window
 */
void window_mainloop(Window* const window)
{
  window->running = 1;
  SDL_Event event;
  // main loop
  while (window->running) {
    // get event
    while (SDL_PollEvent(&event)) window_takeCareOfEvent(window, &event);

    SDL_LockSurface(window->surface);

    if (window->idleCallback) window->idleCallback(window->idleCallbackData);

    SDL_UnlockSurface(window->surface);
    SDL_UpdateWindowSurface(window->window);
  }
}

/**
 * @brief This functions sets event callback function
 *
 * @param window window
 * @param event type of SDL event
 * @param clb callback function
 * @param data user data for callback
 */
void window_setCallback(Window* const window,
                        uint32_t      event,
                        EventCallback clb,
                        void*         data)
{
  assert(window != NULL);
  assert(event < SDL_LASTEVENT);
  window->eventCallback[event] = clb;
  window->callbackData[event]  = data;
}

/**
 * @brief This function sets idle/draw callback function
 *
 * @param window window
 * @param clb function pointer to callback
 * @param data data for callback
 */
void window_setIdleCallback(Window* const window, IdleCallback clb, void* data)
{
  assert(window != NULL);
  window->idleCallback     = clb;
  window->idleCallbackData = data;
}
