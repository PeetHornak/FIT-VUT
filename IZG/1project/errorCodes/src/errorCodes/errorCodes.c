#include <stdint.h>
#include <errorCodes/errorCodes.h>

char const* getErrorMessage(ErrorCode code){
  char const* messages[] = {
    "NO ERROR",
    "stack_firstAlloc failed - cannot allocate memory",
    "stack_realloc failed - cannot reallocate memory",
    "stack_pop failed - stack is empty",
    "queue_push failed - cannot allocate element",
    "queue_pop failed - cannot pop - empty queue",
    "vector_push failed - cannot allocate first element",
    "vector_push failed - cannot reallocate vector",
    "vector operation failed - out of range",
  };
  if((uint32_t)code >= sizeof(messages)/sizeof(char const*))
    return "ERROR CODE IS TOO LARGE!";
  return messages[(uint32_t)code];
}
