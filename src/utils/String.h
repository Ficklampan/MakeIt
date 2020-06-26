#ifndef STRING_H
  #define STRING_H

#include "Type.h"

typedef struct {
  uint32_t length;
  char* ptr;
} string;

string* string_split(string* str, char d, uint32_t* size);

#endif
