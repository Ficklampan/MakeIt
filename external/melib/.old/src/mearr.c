#include "../include/me/mearr.h"

#include <stdlib.h>

static void mearr_realloc(struct array* arr, uint32_t size)
{
  arr->allocated = size;
  arr->entries = realloc(arr->entries, sizeof(void*) * arr->allocated);
}

struct array* mearr_new(uint32_t initial_size)
{
  struct array* arr = calloc(sizeof(struct array), 1);

  if (mearr_init(arr, initial_size) == 0)
  {
    free(arr);
    return NULL;
  }
  return arr;
}

int mearr_init(struct array* arr, uint32_t initial_size)
{
  if (initial_size == 0)
    return 0;

  arr->allocated = initial_size;
  arr->size = 0;
  
  arr->entries = (void**) calloc(sizeof(void*), arr->allocated);
  return 1;
}

void* mearr_push(struct array* arr, void* val)
{
  if (arr->size >= arr->allocated)
    mearr_realloc(arr, arr->allocated * 2);

  arr->entries[arr->size] = val;
  arr->size++;
  return val;
}

struct array* mearr_push_all(struct array* arr, struct array* arr2)
{
  if (arr2->size == 0)
    return arr2;

  if (arr->size + arr2->size > arr->allocated)
    mearr_realloc(arr, arr->size + arr2->size);

  for (uint32_t i = 0; i < arr2->size; i++)
    mearr_push(arr, arr2->entries[i]);

  return arr2;
}

void* mearr_pull(struct array* arr, uint32_t i)
{
  if (i > arr->size)
    return NULL;

  return arr->entries[i];
}

void* mearr_erase(struct array* arr, uint32_t i)
{
  if (i > arr->size)
    return NULL;

  uint32_t index = 0;
  void* value = NULL;
  for (uint32_t j = 0; j < arr->size; j++)
  {
    if (j == i)
    {
      value = arr->entries[j];
      continue;
    }

    arr->entries[index] = arr->entries[j];
    index++;
  }
  arr->size--;
  return value;
}

uint32_t mearr_clear(struct array* arr)
{
  uint32_t size = arr->size;
  arr->size = 0;
  return size;
}

int mearr_free(struct array* arr)
{
  free(arr->entries);
  arr->allocated = 0;
  arr->size = 0;
  return 1;
}

int mearr_freea(struct array* arr)
{
  for (uint32_t i = 0; i < arr->allocated; i++)
  {
    if (arr->entries[i] == NULL)
      continue;

    free(arr->entries[i]);
  }
  return mearr_free(arr);
}
