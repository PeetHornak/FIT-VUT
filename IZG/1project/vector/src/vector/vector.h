/*!
 * @file
 * @brief This file contains declaration of vector.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<stdint.h>
#include<errorCodes/errorCodes.h>
#include<vector/vector_export.h>

/**
 * @brief This struct represents simple vector.
 */
struct Vector{
  void*    data       ;///< value on the vector
  uint32_t size       ;///< vector size
  uint32_t reserved   ;///< reserved memory size in elements
  uint32_t elementSize;///< size of on element in bytes
};

typedef struct Vector Vector;///<shortcut

#ifdef __cplusplus
extern "C" {
#endif

VECTOR_EXPORT void vector_init(Vector*const vector,uint32_t elementSize);

VECTOR_EXPORT void vector_free(Vector*const vector);

VECTOR_EXPORT int vector_empty(Vector const*const vector);

VECTOR_EXPORT uint32_t vector_size(Vector const*const vector);

VECTOR_EXPORT uint32_t vector_elementSize(Vector const*const vector);

VECTOR_EXPORT ErrorCode vector_pushback(Vector*const vector,void const*const value);

VECTOR_EXPORT ErrorCode vector_get(Vector*const vector,uint32_t i,void*const value);

VECTOR_EXPORT ErrorCode vector_set(Vector*const vector,uint32_t i,void const*const value);

VECTOR_EXPORT void* vector_at(Vector*const vector,uint32_t i);

VECTOR_EXPORT void const* vector_atConst(Vector const*const vector,uint32_t i);

VECTOR_EXPORT void* vector_data(Vector*const vector);

#ifdef __cplusplus
}
#endif
