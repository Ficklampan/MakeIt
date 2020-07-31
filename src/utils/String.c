#include "String.h"

#include <stdio.h> // remove
#include <stdlib.h>
#include <string.h>

void string_buffer_init(string_buffer* str_buff, uint32_t initial_size)
{
  str_buff->str = malloc(initial_size);
  str_buff->str[0] = '\0';
  str_buff->length = 0;
  str_buff->allocated = initial_size;
}

void string_buffer_append(string_buffer* str_buff, char c)
{
  if (str_buff->length + 1 >= str_buff->allocated)
  {
    str_buff->allocated*=2;
    str_buff->str = realloc(str_buff->str, str_buff->allocated);
  }
  str_buff->str[str_buff->length++] = c;
  str_buff->str[str_buff->length] = '\0';
}

void string_buffer_clear(string_buffer* str_buff)
{
  str_buff->str[0] = '\0';
  str_buff->length = 0;
}

char** strsplit(char* str, char d, uint32_t* size)
{
  uint32_t len = strlen(str);
  uint32_t length = 1;
  for (uint32_t i = 0; i < len; i++)
  {
    if (str[i] == d)
    {
      str[i] = 0;
      length++;
      continue;
    }
  }
  char** strs = calloc(sizeof(char*), length);
  uint32_t index = 0;
  for (uint32_t i = 0; i < length; i++)
  {
    strs[i] = &str[index];
    index+=strlen(strs[i]) + 1;
  }
  *size = length;
  return strs;
}
char* strapnd(char* str, const char* apnd)
{
  uint32_t len2 = strlen(apnd);
  if (str != NULL)
  {
    uint32_t len1 = strlen(str);
    str = realloc(str, len1 + len2);
    for (uint32_t j = 0; j < len2; j++)
      str[len1 + j] = apnd[j];
  }else
  {
    str = malloc(len2);
    for (uint32_t j = 0; j < len2; j++)
      str[j] = apnd[j];
  }
  return str;
}
char* strapndc(char* str, char c)
{
  if (str != NULL)
  {
    uint32_t len = strlen(str);
    str = realloc(str, len + 1);
    str[len] = c;
  }else
  {
    str = malloc(1);
    str[0] = c;
  }
  return str;
}

// not very pretty
char* strreplace(char* str, const char* what, const char* to, uint32_t* length)
{
  uint32_t len1 = strlen(str);
  uint32_t len2 = strlen(what);
  uint32_t len3 = strlen(to);

  char* output = *length == 0 ? NULL : malloc(*length);
  uint32_t index = 0;

  uint32_t match = 0;
  uint32_t start = 0;
  uint32_t i = 0;
  while (i < len1)
  {
    if (match >= len2)
    {
      for (uint32_t j = 0; j < len3; j++)
      {
        *length++;
        if (output != NULL)
          output[index++] = to[j];
      }
      match = 0;
      start = 0;
      continue;
    }else if (str[i] == what[match])
    {
      if (match == 0)
        start = i;
      match++;
      i++;
      continue;
    }else if (match > 0)
    {
      for (uint32_t j = 0; j < match; j++)
      {
        *length++;
        if (output != NULL)
          output[index++] = str[start + j];
      }
      match = 0;
      start = 0;
    }
    *length++;
    if (output != NULL)
      output[index++] = str[i];
    i++;
  }
  if (output != NULL)
    output[index] = '\0';
  return output;
}
