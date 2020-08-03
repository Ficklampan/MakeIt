#ifndef STRING_H
  #define STRING_H

#include "Type.h"

#include "Map.h"

#include <string.h>

typedef struct {
  char* str;
  uint32_t length, allocated;
} string_buffer;

void string_buffer_init(string_buffer* str_buff, uint32_t initial_size);
void string_buffer_appendc(string_buffer* str_buff, char c);
void string_buffer_append(string_buffer* str_buff, char* str);
void string_buffer_clear(string_buffer* str_buff);
void string_buffer_sync(string_buffer* str_buff);

array* strsplit(char* str, char delimeter);
char* strreplace(const char* str, const char* what, const char* to);
char* strjoin(const char* str1, const char* str2);
char* strtrim(const char* str);
char* strpathfix(const char* str);
char* strdir(const char* str);
char* strfname(char* str);
char* strfext(char* str);
char* strfilext(const char* str, const char* ext);
char* strsub(const char* str, uint32_t start, uint32_t end);
uint32_t strlastiof(const char* str, char c);
bool strcnts(const char* str, const char* what);
char strempty(const char* str);

#endif
