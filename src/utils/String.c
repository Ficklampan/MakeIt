#include "String.h"

#include <stdlib.h>
#include <stdio.h> // remove
#include <string.h>

string string_make(const char* ptr)
{
  string str = {strlen(ptr), ptr};
  return str;
}

string string_empty()
{
  string str = {0, malloc(1)};
  str.ptr[0] = 0;
  return str;
}

string* string_split(string* str, char d, uint32_t* size)
{
  uint32_t length = 1;
  for (uint32_t i = 0; i < str->length; i++)
  {
    if (str->ptr[i] == d)
    {
      str->ptr[i] = 0;
      length++;
      continue;
    }
  }
  string* str_arr = (string*) calloc(sizeof(string), length);
  uint32_t index = 0, i = 0;
  while (index < length)
  {
    uint32_t str_len = strlen(&str->ptr[i]);
    str_arr[index].length = str_len;
    str_arr[index].ptr = &str->ptr[i];
    index++;
    i+=str_len + 1;
  }
  *size = length;
  return str_arr;
}

void string_append(string* str, char c)
{
  str->ptr = realloc(str->ptr, str->length + 2);
  str->ptr[str->length] = c;
  str->ptr[str->length + 1] = 0;
  str->length++;
}

void string_clear(string* str)
{
  if (str->length > 0)
    str->ptr[0] = 0;
  str->length = 0;
}

int string_index(string* str, const char* what)
{
  uint32_t str_len = strlen(what);
  if (str_len > str->length)
    return -1;
  uint32_t index = 0;
  int start_index = -1;
  for (uint32_t i = 0; i < str->length; i++)
  {
    if (str->ptr[i] == what[index])
    {
      if (start_index < 0)
        start_index = index;
      index++;
    }else
      index = 0;
    if (index >= str_len)
      return start_index;
  }
  return -1;
}

void string_replace(string* str, const char* what, const char* to)
{
  // ömm
}
