#include "String.h"

#include <stdlib.h>
#include <stdio.h> // remove
#include <string.h>

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
