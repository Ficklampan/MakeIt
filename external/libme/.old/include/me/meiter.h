#ifndef MELIB_ITER_H
  #define MELIB_ITER_H

#include "metype.h"

struct iterator {
  void** entries;
  uint32_t size;
  uint32_t index;
};

struct iterator* meiter_new(void** entries, uint32_t size);
int meiter_init(struct iterator* iter, void** entries, uint32_t size);
bool meiter_has(struct iterator* iter);
void* meiter_next(struct iterator* iter);
void* meiter_peek(struct iterator* iter);
void* meiter_last(struct iterator* iter);
int meiter_reset(struct iterator* iter);
int meiter_free(struct iterator* iter);
int meiter_freea(struct iterator* iter);

#endif
