#ifndef MAKE_IT_PARSER_H
  #define MAKE_IT_PARSER_H

#include "utils/Map.h"
#include "utils/String.h"

typedef struct {
  char* name;
  array* variables;
} func_element;

void makeit_parser_next_token(array* tokens, string_buffer* str_buffer, char c);
int makeit_parser_parse_data(const char* data, uint32_t data_length, array* elements);

#endif
