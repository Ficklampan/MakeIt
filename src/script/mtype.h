#ifndef MTYPE_H
  #define MTYPE_H

#include "../utils/Type.h"

#include "../utils/Arrays.h"

typedef int mi_int8;
typedef int mi_int16;
typedef int mi_int32;
typedef int mi_int64;
typedef float mi_float32;
typedef float mi_float64;

enum mtoken_t {
  MTK_END_T,
  MTK_COMMA_T,
  MTK_DOT_T,

  MTK_PAR_BEGIN_T,
  MTK_PAR_END_T,

  MTK_SCOPE_BEGIN_T,
  MTK_SCOPE_END_T,

  MTK_PAR_T,
  MTK_SCOPE_T,

  MTK_AT_BEGIN_T,
  MTK_AT_END_T,

  MTK_FUNC_CALL_T,

  MTK_LITERIAL_T,

  MTK_NULL_T,
  MTK_BOOL_T,
  MTK_CHAR_T,
  MTK_INT_T, 
  MTK_LONG_T, 
  MTK_FLOAT_T, 
  MTK_DOUBLE_T,
  MTK_LONG_DOUBLE_T,
  MTK_STRING_T,

  MTK_OPR_ADD_T,
  MTK_OPR_SUB_T,
  MTK_OPR_MUL_T,
  MTK_OPR_DIV_T,
  MTK_OPR_MOD_T,

  MTK_OPR_ADD_VAL_T,
  MTK_OPR_SUB_VAL_T,
  MTK_OPR_MUL_VAL_T,
  MTK_OPR_DIV_VAL_T,
  MTK_OPR_MOD_VAL_T,

  MTK_OPR_INCREMENT_T,
  MTK_OPR_DECREMENT_T,

  MTK_OPR_ASSIGN_T,
  MTK_OPR_EQUAL_T,
  MTK_OPR_NOT_EQUAL_T,

  MTK_OPR_LESS_T,
  MTK_OPR_MORE_T,

  MTK_OPR_LESS_EQUAL_T,
  MTK_OPR_MORE_EQUAL_T,
 
  MTK_OPR_AND_T,
  MTK_OPR_OR_T,
  MTK_OPR_NOT_T,
  MTK_OPR_BIT_NOT_T,
  MTK_OPR_BIT_AND_T,
  MTK_OPR_BIT_OR_T,
  MTK_OPR_BIT_XOR_T,
  MTK_OPR_BIT_LEFT_SHIFT_T,
  MTK_OPR_BIT_RIGHT_SHIFT_T,

};

enum mvar_t {
  MVAR_NULL_T,
  MVAR_VOID_T,
  MVAR_BOOL_T,
  MVAR_INT8_T,
  MVAR_INT16_T,
  MVAR_INT32_T,
  MVAR_INT64_T,
  MVAR_FLOAT32_T,
  MVAR_FLOAT64_T,
  MVAR_STRING_T,
  MVAR_ANY_T
};

enum mstate_t {
  MSTATE_IF_T,
  MSTATE_ELSE_T,
  MSTATE_FOR_T,
  MSTATE_WHILE_T,
  MSTATE_SWITCH_T,
  MSTATE_DO_T
};

typedef struct {
  array* tokens;
  map* functions;
} mscript;

typedef struct {
  enum mtoken_t type;
  uint8_t* value;
} mtoken;

typedef struct {
  mscript* script;
  void* parent;
  map* variables;
} mscope;

typedef struct {
  enum mvar_t type;
  bool constant;
  uint8_t* value;
} mvar;

typedef struct {
  uint32_t argc;
  enum mvar_t* args;
  mvar* (*exec) (array*); 
} mfunc;

typedef struct {
  mfunc* func;
  array* args;
  array* scope;
} mfunc_call;

typedef struct {
  enum mstate_t type;
  mtoken* par_tk;
  mtoken* scope_tk;
} mstatement;

#endif
