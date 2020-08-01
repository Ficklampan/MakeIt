#include "String.h"

#include <stdlib.h>

#include <stdio.h> // remove

void string_buffer_init(string_buffer* str_buff, uint32_t initial_size)
{
  str_buff->str = malloc(initial_size);
  str_buff->str[0] = '\0';
  str_buff->length = 0;
  str_buff->allocated = initial_size;
}

void string_buffer_appendc(string_buffer* str_buff, char c)
{
  if (str_buff->length + 1 >= str_buff->allocated)
  {
    str_buff->allocated*=2;
    str_buff->str = realloc(str_buff->str, str_buff->allocated);
  }
  str_buff->str[str_buff->length++] = c;
  str_buff->str[str_buff->length] = '\0';
}

void string_buffer_append(string_buffer* str_buff, char* str)
{
  uint32_t len = strlen(str);
  for (uint32_t i = 0; i < len; i++)
    string_buffer_appendc(str_buff, str[i]);
}

void string_buffer_clear(string_buffer* str_buff)
{
  str_buff->str[0] = '\0';
  str_buff->length = 0;
}

void string_buffer_sync(string_buffer* str_buff)
{
  str_buff->length = strlen(str_buff->str);
}

char** strsplit(char* str, char delimeter, uint32_t* size)
{
  uint32_t len = strlen(str);
  uint32_t length = 1;
  for (uint32_t i = 0; i < len; i++)
  {
    if (str[i] == delimeter)
    {
      str[i] = 0;
      length++;
      continue;
    }
  }
  char** str_arr = (char**) calloc(sizeof(char*), length);
  uint32_t index = 0, i = 0;
  while (index < length)
  {
    uint32_t str_len = strlen(&str[i]);
    str_arr[index] = &str[i];
    index++;
    i+=str_len + 1;
  }
  *size = length;
  return str_arr;
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
        string_buffer_appendc(output, to[j]);
      i+=len2 - 1;
    }else
      string_buffer_appendc(output, str[i]);
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

char* strtrim(const char* str)
{
  uint32_t len = strlen(str);
  uint32_t start = 0;
  uint32_t end = len;
  while (str[start] == ' ')
    start++;
  while (str[end] == ' ')
    end--;
  char* output = malloc(end - start + 1);
  for (uint32_t i = start; i < end; i++)
    output[i - start] = str[i];

  /* ending the string */
  output[len - start] = '\0';
  return output;
}

char* strpathfix(char* str)
{
  uint32_t len = strlen(str);
  if (str[len - 1] == '/')
    return str;
  char* output = malloc(len + 2);
  for (uint32_t i = 0; i < len; i++)
    output[i] = str[i];
  output[len] = '/';

  /* ending the string */
  output[len + 1] = '\0';
  return output;
}

char* strfilext(const char* str, const char* ext)
{
  uint32_t index = strlastiof(str, '.');
  char* output = strsub(str, 0, index + 1);
  return strjoin(output, ext);
}

char* strsub(const char* str, uint32_t start, uint32_t end)
{
  char* output = malloc(end - start);
  for (uint32_t i = start; i < end; i++)
    output[i - start] = str[i];
  return output;
}

uint32_t strlastiof(const char* str, char c)
{
  uint32_t len = strlen(str);
  for (uint32_t i = 0; i < len; i++)
  {
    if (str[i] == c)
      return i;
  }
  return 0;
}

char strempty(const char* str)
{
  uint32_t len = strlen(str);
  return len == 0 ? 1 : 0;
}
