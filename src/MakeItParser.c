#include "MakeItParser.h"

#include "utils/Type.h"

#include <stdlib.h>
#include <stdio.h>

static void push_string(array* values, string_buffer* str_buffer)
{
  if (str_buffer->length > 0 && !strempty(str_buffer->str))
  {
    char* str = strtrim(str_buffer->str);
    if (!strempty(str_buffer->str))
      array_push(values, str);
    string_buffer_clear(str_buffer);
  }
}

int makeit_parser_parse_data(const char* data, uint32_t data_length, array* elements)
{
  array_init(elements, 32);

  string_buffer* str_buffer = (string_buffer*) calloc(sizeof(string_buffer), 1);
  string_buffer_init(str_buffer, 64);

  func_element* elem = NULL;

  bool ignr = false;

  for (uint32_t i = 0; i < data_length; i++)
  {
    char c = data[i];
    char last = i > 0 ? data[i - 1] : '\0';
    char next = i < data_length - 1 ? data[i + 1] : '\0';

    if (c == '/' && next == '*')
      ignr = true;
    else if (last == '*' && c == '/')
    {
      ignr = false;
      continue;
    }

    if (ignr)
      continue;

    if (c == ':')
    {
      elem = (func_element*) calloc(sizeof(func_element), 1);
      elem->name = (char*) calloc(sizeof(char), str_buffer->length + 1);
      elem->variables = (array*) calloc(sizeof(array), 1);
      array_init(elem->variables, 8);

      strcpy(elem->name, str_buffer->str);
      string_buffer_clear(str_buffer);
      array_push(elements, elem);
      continue;
    }else if (c == '\n' || c == ',')
      push_string(elem->variables, str_buffer);
    else
      string_buffer_appendc(str_buffer, c);
  }
  if (ignr)
  {
    printf(":: ignore flag not ended.\n");
    return 0;
  }
  return 1;
}
