/*!
 * @file
 * @brief This file contains declaration of error codes.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<errorCodes/errorcodes_export.h>

/**
 * @brief ErrorCode for whole application.
 */
enum ErrorCode{
  ERR_OK                 , ///< everything is OK
  ERR_STACK_FIRST_ALLOC  , ///< stack failed at first allocation
  ERR_STACK_REALLOC      , ///< stack failed at reallocation
  ERR_STACK_EMPTY        , ///< stack is empty (cannot pop)
  ERR_QUEUE_ALLOC_ELEMENT, ///< queue cannot allocate element
  ERR_QUEUE_EMPTY        , ///< queue is empty (cannot pop)
  ERR_VECTOR_FIRST_ALLOC , ///< vector failed at first allocation
  ERR_VECTOR_REALLOC     , ///< vector failed at reallocation
  ERR_VECTOR_OUT_OF_RANGE, ///< vector operation out of range
};

typedef enum ErrorCode ErrorCode;///< shortcut


#ifdef __cplusplus
extern "C" {
#endif

ERRORCODES_EXPORT char const*getErrorMessage(ErrorCode code);

#ifdef __cplusplus
}
#endif
