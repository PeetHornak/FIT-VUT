/*!
 * @file
 * @brief This file contains private declaration of stack.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include <stack/stack.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function performs the first allocation of the stack.
 *
 * @param stack stack
 *
 * @return error code
 */
ErrorCode stack_firstAlloc(Stack*const stack);

/**
 * @brief This function performs reallocation of the stack.
 *
 * @param stack stack
 *
 * @return error code
 */
ErrorCode stack_realloc(Stack*const stack);

/**
 * @brief This function resized the stack
 *
 * @param stack stack
 *
 * @return error code
 */
ErrorCode stack_resize(Stack*const stack);

#ifdef __cplusplus
}
#endif
