#ifndef MEM_POOL_H
  #define MEM_POOL_H

#include "utils/Map.h"

typedef struct {
  array* ptrs;
} MemPool;

void mem_pool_init(MemPool* mp, uint32_t initial_size);
void mem_pool_bind(MemPool* mp);
void* mem_alloc(uint32_t size);
void* mem_calloc(uint32_t nitems, uint32_t size);
void* mem_realloc(void* ptr, uint32_t size);
void mem_free(void* ptr);
void mem_afree();

#endif
