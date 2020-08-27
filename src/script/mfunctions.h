#ifndef MI_FUNCS_H
  #define MI_FUNCS_H

#include "mtype.h"
#include "mvar.h"
#include "mfunc.h"

#include "../utils/String.h"

#include <stdio.h>

static inline mvar* func_exec_printf(array* args)
{
  mvar* var = (mvar*) args->values[0];
  if (var->type == MVAR_STRING_T) printf("%s", (char*) var->value);
  else if (var->type == MVAR_INT8_T) printf("%c", *((mi_int8*) var->value));
  else if (var->type == MVAR_INT16_T) printf("%i", *((mi_int16*) var->value));
  else if (var->type == MVAR_INT32_T) printf("%i", *((mi_int32*) var->value));
  else if (var->type == MVAR_INT64_T) printf("%i", *((mi_int64*) var->value));
  else if (var->type == MVAR_FLOAT32_T) printf("%f", *((mi_float32*) var->value));
  else if (var->type == MVAR_FLOAT64_T) printf("%f", *((mi_float64*) var->value));
  else if (var->type == MVAR_BOOL_T) printf("%s", var->value == NULL ? "false" : "true");
  else
  {
    printf(":: unknown data type %s.\n", vtstr(var->type));
    return NULL;
  }
  return vnew(MVAR_VOID_T, true, NULL);
}

static inline mfunc* func_printf()
{
  enum mvar_t* args = (enum mvar_t*) calloc(sizeof(enum mvar_t), 1);
  args[0] = MVAR_ANY_T;
  return fnew(1, args, func_exec_printf);
}

static inline mvar* func_exec_istr(array* args)
{
  mvar* var = (mvar*) args->values[0];
  char s[21];
  sprintf(s, "%d", *((mi_int32*) var->value));
  return vnew(MVAR_STRING_T, true, (uint8_t*) strmem(s));
}

static inline mfunc* func_istr()
{
  enum mvar_t* args = (enum mvar_t*) calloc(sizeof(enum mvar_t), 1);
  args[0] = MVAR_INT32_T;
  return fnew(1, args, func_exec_istr);
}

static inline void func_defaults(map* functions)
{
  map_push(functions, "printf", func_printf());
  map_push(functions, "istr", func_istr());
}

#endif
