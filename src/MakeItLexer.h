#ifndef MAKE_IT_LEXER_H
  #define MAKE_IT_LEXER_H

#include "utils/Arrays.h"
#include "utils/String.h"

typedef struct {
  char* name;
  array* variables;
} func_element;

int MILEX_prsdat(const char* data, uint32_t data_length, array* elements);

#endif
