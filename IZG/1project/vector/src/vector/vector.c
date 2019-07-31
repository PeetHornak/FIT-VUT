/*!
 * @file
 * @brief This file contains implementation of uint32_t vector.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <vector/vector.h>

/**
 * @brief This function inits vector
 *
 * @param vector vector
 * @param elementSize size of one element in bytes
 */
void vector_init(Vector*const vector,uint32_t elementSize){
  assert(vector != NULL);
  assert(elementSize > 0);
  vector->data        = NULL       ;
  vector->size        = 0          ;
  vector->reserved    = 0          ;
  vector->elementSize = elementSize;
}

/**
 * @brief This function frees vector
 *
 * @param vector vector
 */
void vector_free(Vector*const vector){
  assert(vector != NULL);
  if(vector->data)
    free(vector->data);
  vector_init(vector,vector->elementSize);
}

/**
 * @brief This function returns 1 if the vector is empty
 *
 * @param vector vector
 *
 * @return 1 if vector is empty, 0 otherwise
 */
int vector_empty(Vector const*const vector){
  assert(vector != NULL);
  return vector->size == 0;
}

/**
 * @brief This function returns vector size.
 *
 * @param vector vector
 *
 * @return the size of the vector
 */
uint32_t vector_size(Vector const*const vector){
  assert(vector != NULL);
  return vector->size;
}

/**
 * @brief This function returns the element size in bytes
 *
 * @param vector vector
 *
 * @return element size in bytes
 */
uint32_t vector_elementSize(Vector const*const vector){
  assert(vector != NULL);
  return vector->elementSize;
}

ErrorCode vector_firstAlloc(Vector*const vector){
  assert(vector != NULL);
  vector->data = malloc(vector->elementSize);
  if(vector->data == NULL)return ERR_VECTOR_FIRST_ALLOC;
  vector->reserved = 1;
  return ERR_OK;
}

ErrorCode vector_realloc(Vector*const vector){
  assert(vector != NULL);
  void*newPtr = realloc(vector->data,vector->elementSize*vector->reserved*2);
  if(newPtr == NULL)return ERR_VECTOR_REALLOC;
  vector->reserved*=2;
  vector->data = newPtr;
  return ERR_OK;
}

ErrorCode vector_resize(Vector*const vector){
  assert(vector != NULL);
  if(vector->reserved == 0)
    return vector_firstAlloc(vector);
  else
    return vector_realloc(vector);
}

/**
 * @brief This function pushes a new value to the end of the vector.
 *
 * @param vector vector
 * @param value a new value
 *
 * @return return ERR_OK on success
 */
ErrorCode vector_pushback(Vector*const vector,void const*const value){
  assert(vector != NULL);
  if(vector->size >= vector->reserved){
    ErrorCode err = vector_resize(vector);
    if(err != ERR_OK)return err;
  }

  memcpy((uint8_t*)vector->data+vector->size*vector->elementSize,value,vector->elementSize);
  vector->size++;
  return ERR_OK;
}

/**
 * @brief This function gets element.
 *
 * @param vector vector
 * @param i element index
 * @param value element
 *
 * @return returns ERR_OK if successful
 */
ErrorCode vector_get(Vector*const vector,uint32_t i,void*const value){
  assert(vector != NULL);
  if(i >= vector_size(vector))return ERR_VECTOR_OUT_OF_RANGE;
  memcpy(value,(uint8_t*)vector->data+i*vector->elementSize,vector->elementSize);
  return ERR_OK;
}

/**
 * @brief This function sets data to an element
 *
 * @param vector vector
 * @param i index of element
 * @param value data
 */
ErrorCode vector_set(Vector*const vector,uint32_t i,void const*const value){
  assert(vector != NULL);
  if(i >= vector_size(vector))return ERR_VECTOR_OUT_OF_RANGE;
  memcpy((uint8_t*)vector->data+i*vector->elementSize,value,vector->elementSize);
  return ERR_OK;
}

/**
 * @brief This function returns pointer to an element
 *
 * @param vector vector
 * @param i index of the element
 *
 * @return pointer to the element or NULL
 */
void* vector_at(Vector*const vector,uint32_t i){
  assert(vector != NULL);
  if(i >= vector_size(vector))return NULL;
  return (uint8_t*)vector->data + vector->elementSize*i;
}

/**
 * @brief This function returns constant pointer to an element.
 *
 * @param vector vector
 * @param i index of element
 *
 * @return const pointer
 */
void const* vector_atConst(Vector const*const vector,uint32_t i){
  assert(vector != NULL);
  if(i >= vector_size(vector))return NULL;
  return (uint8_t*)vector->data + vector->elementSize*i;
}

/**
 * @brief This function returns pointer to data
 *
 * @param vector vector
 *
 * @return data pointer
 */
void* vector_data(Vector*const vector){
  assert(vector != NULL);
  return vector->data;
}
