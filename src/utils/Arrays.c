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
