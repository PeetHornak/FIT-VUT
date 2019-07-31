/*!
 * @file
 * @brief This file contains implementation of uint32_t stack.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stack/stack.h>

/**
 * @brief This function inits stack
 *
 * @param stack stack
 * @param elementSize size of one element in bytes
 */
void stack_init(Stack*const stack,uint32_t elementSize){
  assert(stack != NULL);
  assert(elementSize > 0);
  stack->values      = NULL       ;
  stack->reserved        = 0          ;
  stack->size    = 0          ;
  stack->elementSize = elementSize;
}

/**
 * @brief This function frees stack
 *
 * @param stack stack
 */
void stack_free(Stack*const stack){
  assert(stack != NULL);
  free(stack->values);
  stack_init(stack,stack->elementSize);
}

/**
 * @brief This function returns 1 if the stack is empty
 *
 * @param stack stack
 *
 * @return 1 if stack is empty, 0 otherwise
 */
int stack_empty(Stack const*const stack){
  assert(stack != NULL);
  return stack->size == 0;
}

/**
 * @brief This function returns the number of size in the stack
 *
 * @param stack stack
 *
 * @return number of size in the stack
 */
uint32_t stack_size(Stack const*const stack){
  assert(stack != NULL);
  return stack->size;
}

/**
 * @brief This function returns the element reserved in bytes
 *
 * @param stack stack
 *
 * @return element reserved in bytes
 */
uint32_t stack_elementSize(Stack const*const stack){
  assert(stack != NULL);
  return stack->elementSize;
}

ErrorCode stack_firstAlloc(Stack*const stack){
  assert(stack != NULL);
  stack->values = malloc(stack->elementSize);
  if(stack->values == NULL)return ERR_STACK_FIRST_ALLOC;
  stack->reserved = 1;
  return ERR_OK;
}

ErrorCode stack_realloc(Stack*const stack){
  assert(stack != NULL);
  void*newPtr = realloc(stack->values,stack->elementSize*stack->reserved*2);
  if(newPtr == NULL)return ERR_STACK_REALLOC;
  stack->reserved*=2;
  stack->values = newPtr;
  return ERR_OK;
}

ErrorCode stack_resize(Stack*const stack){
  assert(stack != NULL);
  if(stack->reserved == 0)
    return stack_firstAlloc(stack);
  else
    return stack_realloc(stack);
}

/**
 * @brief This function pushes a new value to the stack.
 *
 * @param stack stack
 * @param value a new value
 *
 * @return return ERR_OK on success
 */
ErrorCode stack_push(Stack*const stack,void const*const value){
  assert(stack != NULL);
  if(stack->size >= stack->reserved){
    ErrorCode err = stack_resize(stack);
    if(err != ERR_OK)return err;
  }

  memcpy((uint8_t*)stack->values+stack->size*stack->elementSize,value,stack->elementSize);
  stack->size++;
  return ERR_OK;
}

/**
 * @brief This function pops size from the stack
 *
 * @param stack stack
 * @param value the size value of the stack
 *
 * @return return ERR_OK on success
 */
ErrorCode stack_pop(Stack*const stack,void*const value){
  assert(stack != NULL);
  if(stack_empty(stack))return ERR_STACK_EMPTY;
  stack->size--;
  if(value == NULL)return ERR_OK;
  memcpy(value,(uint8_t*)stack->values+stack->size*stack->elementSize,stack->elementSize);
  return ERR_OK;
}

/**
 * @brief This function return pointer to the top of the stack - last element.
 *
 * @param stack stack
 *
 * @return pointer to the top of the stack - last element
 */
void* stack_top(Stack*const stack){
  assert(stack != NULL);
  if(stack_empty(stack))return NULL;
  return (uint8_t*)stack->values+(stack->size-1)*stack->elementSize;
}

/**
 * @brief This function returns pointer to an element
 *
 * @param stack stack
 * @param i index of the element, 0 - deepest
 *
 * @return pointer to the element or NULL
 */
void* stack_at(Stack const*const stack,uint32_t i){
  assert(stack != NULL);
  if(i >= stack_size(stack))return NULL;
  return (uint8_t*)stack->values + stack->elementSize*i;
}

/**
 * @brief This function check if stack contain a value
 *
 * @param stack stack
 * @param d point to value
 *
 * @return 1 if the stack contain the element, 0 otherwise
 */
int stack_contain(Stack const*const stack,void*d){
  assert(stack != NULL);
  assert(d != NULL);
  uint32_t const nofElements = stack_size(stack);
  for(uint32_t i=0;i<nofElements;++i)
    if(memcmp(stack_at(stack,i),d,stack->elementSize) == 0)
      return 1;
  return 0;
}
