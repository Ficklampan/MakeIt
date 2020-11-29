#ifndef MELIB_MAP_H
  #define MELIB_MAP_H

#include "metype.h"

struct map {
  char** keys;
  void** values;
  uint32_t allocated, size;
};

struct map* memap_new(uint32_t initial_size);
int memap_init(struct map* m, uint32_t initial_size);
void* memap_push(struct map* m, char* key, void* value);
void* memap_pull(struct map* m, char* key);
int memap_index(struct map* m, char* key);

/* NOTE: the entry will still be there. if you want to delete it completly you can do 'free(memap_erase("key"));' */
void* memap_erase(struct map* m, char* key);
uint32_t memap_clear(struct map* m);
int memap_free(struct map* m);
int memap_freea(struct map* m);

#endif
