#include "../include/me/memap.h"

#include <stdlib.h>
#include <string.h>

struct map* memap_new(uint32_t initial_size)
{
  struct map* m = calloc(sizeof(struct map), 1);
  
  if (memap_init(m, initial_size) == 0)
  {
    free(m);
    return NULL;
  }
  return m;
}

int memap_init(struct map* m, uint32_t initial_size)
{
  if (initial_size == 0)
    return 0;

  m->allocated = initial_size;
  m->size = 0;

  m->keys = (char**) calloc(sizeof(char*), m->allocated);
  m->values = (void**) calloc(sizeof(void*), m->allocated);
  return 1;
}

void* memap_push(struct map* m, char* key, void* value)
{
  /* check if key already exists */
  uint32_t index = 0;
  for (uint32_t i = 0; i < m->allocated; i++)
  {
    if (index >= m->size)
      break;

    if (m->keys[i] == NULL)
      continue;

    if (strcmp(key, m->keys[i]) == 0)
    {
      m->values[i] = value;
      return value;
    }
    index++;
  }

  /* find an empty slot */
  for (uint32_t i = 0; i < m->allocated; i++)
  {
    if (m->keys[i] == NULL)
    {
      m->keys[i] = key;
      m->values[i] = value;
      m->size++;
      return value;
    }
  }

  uint32_t start = m->allocated;
  m->allocated*=2;

  m->keys = realloc(m->keys, sizeof(char*) * m->allocated);
  m->values = realloc(m->values, sizeof(void*) * m->allocated);

 
  /* set the new memory to zero (only the keys) */
  memset(&m->keys[start], 0, m->allocated - start);

  /* find an empty slot in the new memory */
  for (uint32_t i = start; i < m->allocated; i++)
  {
    if (m->keys[i] == NULL)
    {
      m->keys[i] = key;
      m->values[i] = value;
      m->size++;
      return value;
    }
  }
  return NULL;
}

void* memap_pull(struct map* m, char* key)
{
  uint32_t index = 0;
  for (uint32_t i = 0; i < m->allocated; i++)
  {
    if (index >= m->size)
      return NULL;

    if (m->keys[i] == NULL)
      continue;

    if (strcmp(key, m->keys[i]) == 0)
      return m->values[i];

    index++;
  }
  return NULL;
}

int memap_index(struct map* m, char* key)
{
  uint32_t index = 0;
  for (uint32_t i = 0; i < m->allocated; i++)
  {
    if (index >= m->size)
      return -1;

    if (m->keys[i] == NULL)
      continue;

    if (strcmp(key, m->keys[i]) == 0)
      return index;

    index++;
  }
  return -1;
}

void* memap_erase(struct map* m, char* key)
{
  uint32_t index = 0;
  for (uint32_t i = 0; i < m->allocated; i++)
  {
    if (index >= m->size)
      return NULL;

    if (m->keys[i] == NULL)
      continue;

    if (strcmp(key, m->keys[i]) == 0)
    {
      void* value = m->values[i];
      m->keys[i] = NULL;
      return value;
    }

    index++;
  }
  return NULL;
}

uint32_t memap_clear(struct map* m)
{
  uint32_t cleared = m->size;
  uint32_t index = 0;
  for (uint32_t i = 0; i < m->allocated; i++)
  {
    if (index >= m->size)
      break;

    if (m->keys[i] == NULL)
      continue;

    m->keys[i] = NULL;
    index++;
  }
  m->size = 0;
  return cleared;
}

int memap_free(struct map* m)
{
  free(m->keys);
  free(m->values);
  m->allocated = 0;
  m->size = 0;
  return 0;
}

int memap_freea(struct map* m)
{
  uint32_t index = 0;
  for (uint32_t i = 0; i < m->allocated; i++)
  {
    if (index >= m->size)
      break;

    if (m->keys[i] == NULL)
      continue;

    free(m->keys[i]);
    free(m->values[i]);
    index++;
  }
  return memap_free(m);
}
