#include "MakeItLexer.h"

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

int MILEX_prsdat(const char* data, uint32_t data_length, array* elements)
{
  // TODO:
  return 1;
}
