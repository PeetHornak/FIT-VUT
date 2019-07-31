#pragma once

#include<stdint.h>

#define ELEMENT_FRONT 0
#define ELEMENT_BACK  1

struct Element;
typedef struct Element Element;

struct Element{
  void*   data        ;
  Element*neighbors[2];
};

Element*element_create(uint32_t elementSize);
void element_free(Element*const element);
