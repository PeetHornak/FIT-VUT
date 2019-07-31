/*!
 * @file
 * @brief This file contains declaration of stack.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<stdint.h>
#include<errorCodes/errorCodes.h>
#include<stack/stack_export.h>

/**
 * @brief This struct represents simple uint32_t stack.
 */
struct Stack{
  void*    values     ;///< value on the stack
  uint32_t reserved   ;///< reserved memory size in elements
  uint32_t size       ;///< number of elements in stack
  uint32_t elementSize;///< size of on element in bytes
};

typedef struct Stack Stack;///< shortcut

#ifdef __cplusplus
extern "C" {
#endif

STACK_EXPORT void stack_init(Stack*const stack,uint32_t elementSize);

STACK_EXPORT void stack_free(Stack*const stack);

STACK_EXPORT int stack_empty(Stack const*const stack);

STACK_EXPORT uint32_t stack_size(Stack const*const stack);

STACK_EXPORT uint32_t stack_elementSize(Stack const*const stack);

STACK_EXPORT ErrorCode stack_push(Stack*const stack,void const*const value);

STACK_EXPORT ErrorCode stack_pop(Stack*const stack,void*const value);

STACK_EXPORT void* stack_top(Stack*const stack);

STACK_EXPORT void* stack_at(Stack const*const stack,uint32_t i);

STACK_EXPORT int   stack_contain(Stack const*const stack,void*d);

#ifdef __cplusplus
}
#endif
