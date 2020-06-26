#ifndef STRING_H
  #define STRING_H

#include "Type.h"

typedef struct {
  uint32_t length;
  char* ptr;
} string;

string string_make(const char* ptr);
string string_empty();
string* string_split(string* str, char d, uint32_t* size);
void string_append(string* str, char c);
void string_clear(string* str);
int string_index(string* str, const char* what);
void string_replace(string* str, const char* what, const char* to);

#endif
