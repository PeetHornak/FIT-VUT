#include <assert.h>
#include <stdlib.h>
#include <student/callbackAndData.h>

/**
 * @brief This function inits callbackAndData structure.
 *
 * @param c callbackAndData structure
 */
void callbackAndData_init(CallbackAndData*const c){
  assert(c != NULL);
  c->callback = NULL;
  c->data     = NULL;
}

/**
 * @brief This function calls callbackAndData structure
 *
 * @param c callbackAndData structure
 */
void call(CallbackAndData*const c){
  assert(c != NULL);
  if(!c->callback)return;
  c->callback(c->data);
}

