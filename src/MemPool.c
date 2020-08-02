#include "MemPool.h"

#include <stdlib.h>

static MemPool* mem_pool = NULL;

static void* push_ptr(void* ptr)
{
  array_push(mem_pool->ptrs, ptr);
  return ptr;
}

static void pull_ptr(void* ptr)
{
  for (uint32_t i = 0; i < mem_pool->ptrs->used; i++)
  {
    if (mem_pool->ptrs->values[i] == ptr)
      mem_pool->ptrs->values[i] = NULL;
  }
}

void mem_pool_init(MemPool* mp, uint32_t initial_size)
{
  mp->ptrs = (array*) calloc(sizeof(array), 1);
  array_init(mp->ptrs, initial_size);
}

void mem_pool_bind(MemPool* mp)
{
  mem_pool = mp;
}

void* mem_alloc(uint32_t size)
{
  return push_ptr(malloc(size));
}

void* mem_calloc(uint32_t nitems, uint32_t size)
{
  return push_ptr(calloc(nitems, size));
}

void* mem_realloc(void* ptr, uint32_t size)
{
  mem_free(ptr);
  return push_ptr(realloc(ptr, size));
}

void mem_free(void* ptr)
{
  pull_ptr(ptr);
  free(ptr);
}

void mem_afree()
{
  for (uint32_t i = 0; i < mem_pool->ptrs->used; i++)
  {
    free(mem_pool->ptrs->values[i]);
    mem_pool->ptrs->values[i] = NULL;
  }
}
