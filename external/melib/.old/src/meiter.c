#include "../include/me/meiter.h"

#include <stdlib.h>

struct iterator* meiter_new(void** entries, uint32_t size)
{
  struct iterator* iter = calloc(sizeof(struct iterator), 1);

  if (meiter_init(iter, entries, size) == 0)
  {
    free(iter);
    return NULL;
  }
  return iter;
}

int meiter_init(struct iterator* iter, void** entries, uint32_t size)
{
  if (entries == NULL || size == 0)
    return 0;

  iter->entries = entries;
  iter->size = size;
  iter->index = 0;
  return 1;
}

bool meiter_has(struct iterator* iter)
{
  return iter->index < iter->size;
}

void* meiter_next(struct iterator* iter)
{
  return iter->entries[iter->index++];
}

void* meiter_peek(struct iterator* iter)
{
  return iter->entries[iter->index];
}

void* meiter_last(struct iterator* iter)
{
  if (iter->index == 0)
    return NULL;
  return iter->entries[iter->index - 1];
}

int meiter_reset(struct iterator* iter)
{
  iter->index = 0;
  return 1;
}

int meiter_free(struct iterator* iter)
{
  free(iter->entries);
  iter->size = 0;
  return 1;
}

int meiter_freea(struct iterator* iter)
{
  for (uint32_t i = 0; i < iter->size; i++)
    free(iter->entries[i]);
  return meiter_free(iter);
}
