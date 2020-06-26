#ifndef LIST_H
  #define LIST_H

#include "Type.h"

typedef struct {
  uint32_t element;
  uint32_t length;
  void* ptr;
} list;

void list_create(list* l, uint32_t element);
void list_push(list* l, void* ptr);
void* list_pull(list* l, uint32_t index);

#endif
