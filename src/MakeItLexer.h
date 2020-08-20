#ifndef MAKE_IT_LEXER_H
  #define MAKE_IT_LEXER_H

#include "utils/Arrays.h"
#include "utils/String.h"

#define DECIMAL_SEPERATOR		'.'
#define EXPR_END 			'\n'

typedef struct {
  char* name;
  array* variables;
} func_element;

enum token_t {
  TT_END,
  TT_SEPERATOR,
  TT_DOT,
  TT_ARGS_BEGIN,
  TT_ARGS_END,
  TT_AT,
  TT_AT_END,

  TT_NAME, 
  TT_STRING, 

  TT_INT, 
  TT_LONG, 
  TT_FLOAT, 
  TT_DOUBLE, 

  TT_BOOL,

  TT_OPR_ADD,
  TT_OPR_SUB,
  TT_OPR_MUL,
  TT_OPR_DIV,
  TT_OPR_MOD,

  TT_OPR_ADDV,
  TT_OPR_SUBV,
  TT_OPR_MULV,
  TT_OPR_DIVV,
  TT_OPR_MODV,

  TT_OPR_INCREMENT,
  TT_OPR_DECREMENT,

  TT_OPR_ASSIGN,
  TT_OPR_EQUAL,

  TT_OPR_NOT_EQUAL,
  TT_OPR_LESS,
  TT_OPR_MORE,

  TT_OPR_LESS_EQUAL,
  TT_OPR_MORE_EQUAL,
 
  TT_OPR_AND,
  TT_OPR_OR,
  TT_OPR_NOT,
  
  TT_OPR_BIT_NOT,
  TT_OPR_BIT_AND,
  TT_OPR_BIT_OR,
  TT_OPR_BIT_XOR,
  TT_OPR_BIT_LEFT_SHIFT,
  TT_OPR_BIT_RIGHT_SHIFT,

};

typedef struct {
  enum token_t type;
  void* value;
} token;

int MILEX_prsdat(const char* data, uint32_t data_length, array* elements);
int MILEX_procdat(array* tokens, string_buffer* compiled);

#endif
