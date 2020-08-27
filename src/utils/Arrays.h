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

typedef struct {
  array* arr;
  uint32_t index;
} iterator;

void map_init(map* m, uint32_t initial_size);
void* map_push(map* m, char* key, void* value);
void* map_pull(map* m, const char* key);
map* map_new(uint32_t initial_size);

void array_init(array* a, uint32_t initial_size);
void array_push(array* a, void* value);
void array_clear(array* a);
array* array_new(uint32_t initial_size);
void array_delete(array* a);

void iter_init(iterator* i, array* arr);
bool iter_has(iterator* i);
void* iter_next(iterator* i);
void* iter_peek(iterator* i);
iterator* iter_new(array* arr);

#endif
