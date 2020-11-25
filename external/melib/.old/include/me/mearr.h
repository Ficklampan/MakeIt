#ifndef MELIB_STK_H
  #define MELIB_STK_H

#include "metype.h"

struct array {
  void** entries;
  uint32_t allocated, size;
};

struct array* mearr_new(uint32_t initial_size);
int mearr_init(struct array* arr, uint32_t initial_size);
void* mearr_push(struct array* arr, void* val);
struct array* mearr_push_all(struct array* arr, struct array* arr2);
void* mearr_pull(struct array* arr, uint32_t i);
void* mearr_erase(struct array* arr, uint32_t i);
uint32_t mearr_clear(struct array* arr);
int mearr_free(struct array* arr);
int mearr_freea(struct array* arr);

#endif
