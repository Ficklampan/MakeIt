#ifndef MAP_H
  #define MAP_H

#include "Type.h"

typedef struct {
  char** keys;
  void** values;
  uint32_t used, size;
} map;

typedef struct {
  void** values;
  uint32_t used, size;
} array;

void map_init(map* m, uint32_t initial_size);
void map_push(map* m, const char* key, const void* value);
void* map_pull(map* m, const char* key);

void array_init(array* a, uint32_t initial_size);
void array_push(array* a, const void* value);
void array_clear(array* a);

#endif
