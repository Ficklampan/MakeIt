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

array* strsplit(char* str, char delimeter)
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
  array* arr = (array*) calloc(sizeof(array), 1);
  array_init(arr, length);
  uint32_t index = 0, i = 0;
  while (index < length)
  {
    uint32_t str_len = strlen(&str[i]);
    array_push(arr, &str[i]);
    index++;
    i+=str_len + 1;
  }
  return arr;
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

  char* joined = (char*) calloc(sizeof(char), len1 + len2);
  uint32_t index = 0;
  for (uint32_t i = 0; i < len1; i++)
    joined[index++] = str1[i];
  for (uint32_t i = 0; i < len2; i++)
    joined[index++] = str2[i];
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

char* strpathfix(const char* str)
{
  uint32_t len = strlen(str);
  char* output = (char*) malloc(len + 1);
  for (uint32_t i = 0; i < len; i++)
    output[i] = str[i];
  output[len] = '\0';
  return output;
}

char* strdir(const char* str)
{
  uint32_t end = strlastiof(str, '/');
  return strsub(str, 0, end);
}

char* strfname(char* str)
{
  uint32_t start = strlastiof(str, '/');
  return strsub(str, start + 1, strlen(str));
}

char* strfilext(const char* str, const char* ext)
{
  uint32_t index = strlastiof(str, '.');
  char* output = strsub(str, 0, index + 1);
  return strjoin(output, ext);
}

char* strsub(const char* str, uint32_t start, uint32_t end)
{
  char* output = (char*) calloc(sizeof(char), end - start);
  for (uint32_t i = start; i < end; i++)
    output[i - start] = str[i];
  return output;
}

uint32_t strlastiof(const char* str, char c)
{
  uint32_t len = strlen(str);
  for (uint32_t i = len - 1; i >= 0; i--)
  {
    if (str[i] == c)
      return i;
  }
  return 0;
}

bool strcnts(const char* str, const char* what)
{
  uint32_t len1 = strlen(str);
  uint32_t len2 = strlen(what);

  uint32_t j = 0;
  for (uint32_t i = 0; i < len1; i++)
  {
    if (str[i] == what[j])
    {
      j++;
      if (j >= len2)
        return true;
    }else
      j = 0;
  }
  return false;
}

char strempty(const char* str)
{
  uint32_t len = strlen(str);
  return len == 0 ? 1 : 0;
}
