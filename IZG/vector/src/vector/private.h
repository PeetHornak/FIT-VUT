/*!
 * @file
 * @brief This file contains private declaration of vector.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include <vector/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function performs the first allocation of a vector.
 *
 * @param vector vector
 *
 * @return error code
 */
ErrorCode vector_firstAlloc(Vector*const vector);

/**
 * @brief This functions performs the reallocation of a vector.
 *
 * @param vector vector
 *
 * @return error code
 */
ErrorCode vector_realloc(Vector*const vector);

/**
 * @brief This function resizes a vector
 *
 * @param vector vector
 *
 * @return error code
 */
ErrorCode vector_resize(Vector*const vector);

#ifdef __cplusplus
}
#endif
