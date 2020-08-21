#ifndef MAKE_IT_LEXER_H
  #define MAKE_IT_LEXER_H

#include "utils/Arrays.h"
#include "utils/String.h"

#define DECIMAL_SEPERATOR		'.'
#define EXPR_END 			'\n'

#define VAR_CHAR_T			1
#define VAR_SHORT_T			2
#define VAR_INT_T			3
#define VAR_LONG_T			4
#define VAR_FLOAT_T			5
#define VAR_DOUBLE_T			6
#define VAR_BOOL_T			7

#define VAR_STRING_T			8

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

typedef struct {
  map* variables;
} script;

typedef struct {
  uint8_t type;
  void* value;
} variable;

int MILEX_prsdat(const char* data, uint32_t data_length, array* elements);
int MILEX_proctokens(array* tokens);

int MILEX_procvar(iterator* iter, script* s);
variable* MILEX_procopr(enum token_t type, variable* var, iterator* iter, script* s);

uint8_t* MILEX_procval(iterator* iter, uint8_t* type, script* s);

uint8_t* MILEX_procopra(enum token_t t, uint8_t* type, uint8_t* value, uint8_t* from);

uint32_t MILEX_sizeof(int type);

#endif
