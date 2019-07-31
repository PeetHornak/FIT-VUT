/*!
 * @file
 * @brief This file contains implementation of uint32_t queue.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <queue/queue.h>
#include <queue/private/element.h>

/**
 * @brief This function inits queue
 *
 * @param queue queue
 * @param elementSize size of one element in bytes
 */
void queue_init(Queue*const queue,uint32_t elementSize){
  assert(queue != NULL);
  assert(elementSize > 0);
  queue->ends[QUEUE_FRONT] = NULL;
  queue->ends[QUEUE_BACK ] = NULL;
  queue->elements    = 0          ;
  queue->elementSize = elementSize;
}

/**
 * @brief This function frees queue
 *
 * @param queue queue
 */
void queue_free(Queue*const queue){
  assert(queue != NULL);
  while(queue->ends[QUEUE_FRONT] != NULL){
    Element*next = queue->ends[QUEUE_FRONT]->neighbors[ELEMENT_BACK];
    element_free(queue->ends[QUEUE_FRONT]);
    queue->ends[QUEUE_FRONT] = next;
  }
  queue_init(queue,queue->elementSize);
}

/**
 * @brief This function returns 1 if the queue is empty
 *
 * @param queue queue
 *
 * @return 1 if queue is empty, 0 otherwise
 */
int queue_empty(Queue const*const queue){
  assert(queue != NULL);
  return queue->elements == 0;
}

/**
 * @brief This function returns the number of elements in the queue
 *
 * @param queue queue
 *
 * @return number of elements in the queue
 */
uint32_t queue_elements(Queue const*const queue){
  assert(queue != NULL);
  return queue->elements;
}

/**
 * @brief This function returns the element size in bytes
 *
 * @param queue queue
 *
 * @return element size in bytes
 */
uint32_t queue_elementSize(Queue const*const queue){
  assert(queue != NULL);
  return queue->elementSize;
}

/**
 * @brief This function pushes a new value to the end of the queue.
 *
 * @param queue queue
 * @param value a new value
 *
 * @return return ERR_OK on success
 */
ErrorCode queue_push(Queue*const queue,void*const value){
  assert(queue != NULL);

  Element*elem = element_create(queue->elementSize);
  if(elem == NULL)return ERR_QUEUE_ALLOC_ELEMENT;
  memcpy((uint8_t*)elem->data,value,queue->elementSize);

  elem->neighbors[ELEMENT_FRONT] = queue->ends[QUEUE_BACK];
  elem->neighbors[ELEMENT_BACK ] = NULL;

  if(queue_empty(queue))
    queue->ends[QUEUE_FRONT] = elem;
  else
    queue->ends[QUEUE_BACK]->neighbors[ELEMENT_BACK] = elem;
  queue->ends[QUEUE_BACK] = elem;

  queue->elements++;
  return ERR_OK;
}

/**
 * @brief This function pops elements from the front of the queue
 *
 * @param queue queue
 * @param value the elements value of the queue
 *
 * @return return ERR_OK on success
 */
ErrorCode queue_pop(Queue*const queue,void*const value){
  assert(queue != NULL);

  if(queue_empty(queue))return ERR_QUEUE_EMPTY;

  Element*elem = queue->ends[QUEUE_FRONT];
  queue->ends[QUEUE_FRONT] = elem->neighbors[ELEMENT_BACK];

  if(queue->ends[QUEUE_FRONT])
    queue->ends[QUEUE_FRONT]->neighbors[ELEMENT_FRONT] = NULL;
  else
    queue->ends[QUEUE_BACK] = NULL;

  if(value != NULL)
    memcpy(value,(uint8_t*)elem->data,queue->elementSize);

  element_free(elem);

  queue->elements--;
  return ERR_OK;
}

/**
 * @brief This function returns pointer to one of the ends of the queue
 *
 * @param queue queue
 * @param end queue end
 *
 * @return pointer to queue end element
 */
void* queue_getEnd(Queue const*const queue,int end){
  assert(queue != NULL);
  assert(end < 2);
  if(queue_empty(queue))return NULL;
  return (uint8_t*)queue->ends[end]->data;
}

/**
 * @brief This function return front element data.
 *
 * @param queue queue
 *
 * @return front element data or NULL if the queue is empty
 */
void* queue_front(Queue*const queue){
  return queue_getEnd(queue,QUEUE_FRONT);
}

/**
 * @brief This function return back element data.
 *
 * @param queue queue
 *
 * @return back element data or NULL if the queue is empty
 */
void* queue_back(Queue*const queue){
  return queue_getEnd(queue,QUEUE_BACK);
}
