#include "Arrays.h"

#include <stdlib.h>
#include <string.h>

void map_init(map* m, uint32_t initial_size)
{
  m->keys = malloc(initial_size * sizeof(char*));
  m->values = malloc(initial_size * sizeof(void*));
  m->size = initial_size;
  m->used = 0;
}

void* map_push(map* m, char* key, void* value)
{
  bool has = false;
  uint32_t index = 0;

  for (uint32_t i = 0; i < m->used; i++)
  {
    if (strcmp(m->keys[i], key) == 0)
    {
      has = true;
      index = i;
      break;
    }
  }

  if (has)
  {
    m->values[index] = value;
    return value;
  }

  if (m->used >= m->size)
  {
    m->size*=2;
    m->keys = realloc(m->keys, m->size * sizeof(char*));
    m->values = realloc(m->values, m->size * sizeof(void*));
  }
  m->keys[m->used] = key;
  m->values[m->used++] = value;
  return value;
}

void* map_pull(map* m, const char* key)
{
  for (uint32_t i = 0; i < m->used; i++)
  {
    if (strcmp(m->keys[i], key) == 0)
      return m->values[i];
  }
  return NULL;
}

map* map_new(uint32_t initial_size)
{
  map* m = (map*) calloc(sizeof(map), 1);
  map_init(m, initial_size);
  return m;
}

void array_init(array* a, uint32_t initial_size)
{
  a->values = malloc(initial_size * sizeof(void*));
  a->size = initial_size;
  a->used = 0;
}

void array_push(array* a, void* value)
{
  if (a->used >= a->size)
  {
    a->size*=2;
    a->values = realloc(a->values, a->size * sizeof(void*));
  }
  a->values[a->used++] = value;
}

void array_clear(array* a)
{
  a->used = 0;
}

array* array_new(uint32_t initial_size)
{
  array* arr = (array*) calloc(sizeof(array), 1);
  array_init(arr, initial_size);
  return arr;
}

void array_delete(array* a)
{
  free(a->values);
  free(a);
}

void iter_init(iterator* i, array* arr)
{
  i->arr = arr;
  i->index = 0;
}

bool iter_has(iterator* i)
{
  return i->index < i->arr->used;
}

void* iter_next(iterator* i)
{
  if (!iter_has(i))
    return NULL;
  return i->arr->values[i->index++];
}

void* iter_peek(iterator* i)
{
  if (i->index >= i->arr->used)
    return NULL;
  return i->arr->values[i->index];
}

void iter_reset(iterator* i)
{
  i->index = 0;
}

iterator* iter_new(array* arr)
{
  iterator* iter = (iterator*) calloc(sizeof(iterator), 1);
  iter_init(iter, arr);
  return iter;
}
