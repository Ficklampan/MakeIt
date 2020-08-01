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

// not very pretty
char* strreplace(const char* str, const char* what, const char* to)
{
  uint32_t len1 = strlen(str);
  uint32_t len2 = strlen(what);
  uint32_t len3 = strlen(to);

  string_buffer* output = calloc(sizeof(string_buffer), 1);
  string_buffer_init(output, len1);

  for (uint32_t i = 0; i < len1; i++)
  {
    char match = (i < len1 - len2 + 1) ? 1 : 0;
    for (uint32_t j = 0; j < len2 && (i + j) < len1; j++)
    {
      if (str[i + j] != what[j])
      {
        match = 0;
        break;
      }
    }
    if (match == 1)
    {
      for (uint32_t j = 0; j < len3; j++)
        string_buffer_append(output, to[j]);
      i+=len2 - 1;
    }else
      string_buffer_append(output, str[i]);
  }
  return output->str;
}

char* strjoin(const char* str1, const char* str2)
{
  uint32_t len1 = strlen(str1);
  uint32_t len2 = strlen(str2);



  char* joined = malloc(len1 + len2);
  for (uint32_t i = 0; i < len1; i++)
    joined[i] = str1[i];
  for (uint32_t i = 0; i < len2; i++)
    joined[i + len1] = str2[i];
  return joined;
}
