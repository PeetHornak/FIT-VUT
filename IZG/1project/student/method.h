#pragma once

#include <student/callbackAndData.h>

/**
 * @brief This struct represents one method of rendering a scene.
 */
struct Method{
  CallbackAndData onInit;///< on init callback - called once at the start
  CallbackAndData onDraw;///< on draw callback - called every frame
  CallbackAndData onExit;///< on exit callback - called once at the end
  char const*     name  ;///< method's name
};

typedef struct Method Method;///< shortcut

void method_init(Method*const m);

