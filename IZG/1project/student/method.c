#include <assert.h>
#include <stdlib.h>
#include <student/method.h>

/**
 * @brief This function inits method.
 *
 * @param m method
 */
void method_init(Method*const m){
  assert(m != NULL);
  callbackAndData_init(&m->onInit);
  callbackAndData_init(&m->onDraw);
  callbackAndData_init(&m->onExit);
  m->name = "";
}

