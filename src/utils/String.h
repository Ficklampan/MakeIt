#ifndef STRING_H
  #define STRING_H

#include "Type.h"

typedef struct {
  char* str;
  uint32_t length, allocated;
} string_buffer;

void string_buffer_init(string_buffer* str_buff, uint32_t initial_size);
void string_buffer_append(string_buffer* str_buff, char c);
void string_buffer_clear(string_buffer* str_buff);

char* strreplace(const char* str, const char* what, const char* to);
char* strjoin(const char* str1, const char* str2);

#endif
