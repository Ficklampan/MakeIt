#ifndef MTYPE_H
  #define MTYPE_H

#include "../utils/Type.h"
#include "../utils/Arrays.h"

#include <stdlib.h>

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
  MTK_GROUP_T,
  MTK_SCOPE_T,
  MTK_CALL_T,
  MTK_LITERIAL_T,
  MTK_VALUE_T,
  MTK_OPERATOR_T,
  MTK_STATEMENT_T
};

enum mopr_t {
  MOPR_BOOL_AND_T,
  MOPR_BOOL_OR_T,
  MOPR_BOOL_NOT_T,
  MOPR_BOOL_MORE_T,
  MOPR_BOOL_LESS_T,
  MOPR_BOOL_MORE_EQL_T,
  MOPR_BOOL_LESS_EQL_T,

  MOPR_ASSIGN_T,

  MOPR_INCREMENT_T,
  MOPR_DECREMENT_T,

  MOPR_EQL_EQUALS_T,
  MOPR_EQL_NOT_EQUALS_T,

  MOPR_ART_ADD_T,
  MOPR_ART_SUB_T,
  MOPR_ART_MUL_T,
  MOPR_ART_DIV_T,
  MOPR_ART_MOD_T,

  MOPR_AA_ADD_T,
  MOPR_AA_SUB_T,
  MOPR_AA_MUL_T,
  MOPR_AA_DIV_T,
  MOPR_AA_MOD_T,
  MOPR_AA_AND_T,
  MOPR_AA_OR_T,
  MOPR_AA_XOR_T,
  MOPR_AA_SHIFT_LEFT_T,
  MOPR_AA_SHIFT_RIGHT_T,

  MOPR_BIT_NOT_T,
  MOPR_BIT_AND_T,
  MOPR_BIT_OR_T,
  MOPR_BIT_XOR_T,
  MOPR_BIT_SHIFT_LEFT_T,
  MOPR_BIT_SHIFT_RIGHT_T,
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
  MVAR_ARRAY_T,
  MVAR_RECORD_T,
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
  uint32_t lpos, cpos;
  uint32_t start, end;

  char* file;
  char* source;
  uint32_t slength;
} mtoken_l;

typedef struct {
  array* tokens;
  map* functions;
} mscript;

typedef struct {
  enum mtoken_t type;
  void* value;
  mtoken_l* location;
} mtoken;

typedef struct {
  mscript* script;
  void* parent;
  map* variables;
} mscope;

typedef struct {
  enum mvar_t type;
  bool constant;
  void* value;
} mvar;

typedef struct {
  enum mvar_t type;
  array* arr;
} marray;

typedef struct {
  uint32_t size;
  void* ptr;
} mrecord;

typedef struct {
  uint32_t argc;
  enum mvar_t* args;
  mvar* (*exec) (mscript*, mscope*, array*); 
} mfunc;

typedef struct {
  mfunc* func;
  array* args;
} mfunc_call;

typedef struct {
  enum mstate_t type;
  mtoken* par_tk;
  mtoken* scope_tk;
} mstatement;

static inline mvar* mscope_pull(mscope* scope, char* name, mscope** location)
{
  mvar* var = (mvar*) map_pull(scope->variables, name);
  if (var == NULL && scope->parent != NULL)
    return mscope_pull((mscope*) scope->parent, name, location);
  if (location != NULL)
    *location = scope;
  return var;
}

static inline mscope* mscope_new(mscript* script, mscope* parent)
{
  mscope* scope = (mscope*) calloc(sizeof(mscope), 1);
  scope->script = script;
  scope->parent = parent;
  scope->variables = map_new(16);
  return scope;
}

static inline mtoken_l* mtloc_new(uint32_t lpos, uint32_t cpos, uint32_t start, uint32_t end, char* file, char* source, uint32_t slength)
{
  mtoken_l* loc = (mtoken_l*) calloc(sizeof(mtoken_l), 1);
  loc->lpos = lpos;
  loc->cpos = cpos;
  loc->start = start;
  loc->end = end;
  loc->file = file;
  loc->source = source;
  loc->slength = slength;
  return loc;
}

static inline marray* marray_new(enum mvar_t type, array* arr)
{
  marray* array = (marray*) calloc(sizeof(marray), 1);
  array->type = type;
  array->arr = arr;
  return array;
}

static inline mrecord* mrecord_new(uint32_t size, void* ptr)
{
  mrecord* record = (mrecord*) calloc(sizeof(mrecord), 1);
  record->size = size;
  record->ptr = ptr;
  return record;
}

#endif
