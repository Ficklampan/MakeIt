#include "List.h"

#include <stdlib.h>

void list_create(list* l, uint32_t element)
{
  l->element = element;
  l->length = 0;
  l->ptr = NULL;
}
void list_push(list* l, void* ptr)
{
  uint32_t new_size = l->length + l->element;
  l->ptr = realloc(l->ptr, new_size);
}
void* list_pull(list* l, uint32_t index)
{
  return &l->ptr[l->element * index];
}
