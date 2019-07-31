#include<stdlib.h>
#include <queue/private/element.h>

Element*element_create(uint32_t elementSize){
  Element*elem = (Element*)malloc(sizeof(Element));
  if(elem == NULL)return NULL;
  elem->data = malloc(elementSize);
  if(elem->data == NULL){
    element_free(elem);
    return NULL;
  }
  return elem;
}

void element_free(Element*const element){
  if(element == NULL)return;
  if(element->data)free(element->data);
  element->data = NULL;
  free(element);
}
