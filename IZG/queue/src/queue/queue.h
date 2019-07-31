/*!
 * @file
 * @brief This file contains declaration of queue.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<stdint.h>
#include<errorCodes/errorCodes.h>
#include<queue/fwd.h>
#include<queue/queue_export.h>

/// front element of the queue
#define QUEUE_FRONT 0
/// back element of the queue
#define QUEUE_BACK  1

/**
 * @brief This struct represents queue.
 */
struct Queue{
  Element* ends[2]    ; ///< front and back element of the queue
  uint32_t elements   ; ///< number of elements in the queue
  uint32_t elementSize; ///< element size 
};

#ifdef __cplusplus
extern "C" {
#endif

QUEUE_EXPORT void      queue_init       (Queue      *const queue,uint32_t   elementSize);
QUEUE_EXPORT void      queue_free       (Queue      *const queue                       );
QUEUE_EXPORT int       queue_empty      (Queue const*const queue                       );
QUEUE_EXPORT uint32_t  queue_elements   (Queue const*const queue                       );
QUEUE_EXPORT uint32_t  queue_elementSize(Queue const*const queue                       );
QUEUE_EXPORT ErrorCode queue_push       (Queue      *const queue,void*const value      );
QUEUE_EXPORT ErrorCode queue_pop        (Queue      *const queue,void*const value      );
QUEUE_EXPORT void*     queue_front      (Queue      *const queue                       );
QUEUE_EXPORT void*     queue_back       (Queue      *const queue                       );

#ifdef __cplusplus
}
#endif
