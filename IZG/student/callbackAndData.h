#pragma once

/**
 * @brief This type represents callback with one parameter
 */
typedef void(*Callback)(void*);

/**
 * @brief This struct represents callback with user data.
 */
struct CallbackAndData{
  Callback callback; ///< callback function
  void*    data    ; ///< callback user data
};

typedef struct CallbackAndData CallbackAndData;///< shortcut

void callbackAndData_init(CallbackAndData*const c);
void call(CallbackAndData*const c);

