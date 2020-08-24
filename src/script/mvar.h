#ifndef MI_VAR_H
  #define MI_VAR_H

#include "mtype.h"

#include "mconf.h"
#include "mtoken.h"

#include "../utils/Type.h"

#include <stdlib.h>
#include <stdio.h>

#define nopr(op, v1, v2) (v1 op v2)

#define MI_BOOL_TRUE		NULL + 8
#define MI_BOOL_FALSE		NULL

static inline char* vtstr(enum mvar_t type)
{
  switch (type)
  {
    case MVAR_NULL_T: return "null"; break;
    case MVAR_VOID_T: return "void"; break;
    case MVAR_BOOL_T: return "bool"; break;
    case MVAR_INT8_T: return "char"; break;
    case MVAR_INT16_T: return "short"; break;
    case MVAR_INT32_T: return "int"; break;
    case MVAR_FLOAT32_T: return "float"; break;
    case MVAR_FLOAT64_T: return "double"; break;
    case MVAR_STRING_T: return "string"; break;
    case MVAR_ANY_T: return "auto"; break;
    default:
      return "?";
    break;
  }
}

static inline uint32_t vsizeof(enum mvar_t type)
{
  /* TODO: add more here */
  if (type == MVAR_BOOL_T) return 1;
  else if (type == MVAR_INT8_T) return sizeof(char);
  else if (type == MVAR_INT16_T) return sizeof(short);
  else if (type == MVAR_INT32_T) return sizeof(int);
  else if (type == MVAR_FLOAT32_T) return sizeof(float);
  else if (type == MVAR_FLOAT64_T) return sizeof(double);
  return 0;
}

static inline mvar* vnew(enum mvar_t type, bool constant, uint8_t* value)
{
  mvar* var = (mvar*) calloc(sizeof(mvar), 1);
  var->type = type;
  var->constant = constant;
  var->value = value;
  return var;
}

static inline mvar* vtoken(mvar* var, mtoken* t)
{
  enum mvar_t type = MVAR_VOID_T;
  if (t->type == MTK_NULL_T) type = MVAR_NULL_T;
  else if (t->type == MTK_BOOL_T) type = MVAR_BOOL_T;
  else if (t->type == MTK_CHAR_T) type = MVAR_INT8_T;
  else if (t->type == MTK_INT_T) type = MVAR_INT32_T;
  else if (t->type == MTK_FLOAT_T) type = MVAR_FLOAT32_T;
  else if (t->type == MTK_DOUBLE_T) type = MVAR_FLOAT64_T;
  else if (t->type == MTK_STRING_T) type = MVAR_STRING_T;
  var->type = type;
  var->value = (uint8_t*) t->value;
  return var;
}

static inline mvar* vcpy(mvar* dest, mvar* var)
{
  uint32_t size = vsizeof(var->type);
  dest->value = (uint8_t*) calloc(size, 1);
  dest->type = var->type;

  for (uint32_t i = 0; i < size; i++)
    dest->value[i] = var->value[i];
  return dest;
}

static inline int vassignc(mvar* var, uint8_t* value)
{
  var->value = value;
  return 1;
}

static inline int vassign(mvar* var, uint8_t* value)
{
  if (var->value != NULL && var->constant)
  {
    printf(wstr(0x02), "d");
    return 0;
  }
  return vassignc(var, value);
}

static inline mi_float32* vcastf32(mvar* v)
{
  if (v->type == MVAR_INT8_T) return (mi_float32*) (mi_int8*) v->value;
  if (v->type == MVAR_INT16_T) return (mi_float32*) (mi_int16*) v->value;
  if (v->type == MVAR_INT32_T) return (mi_float32*) (mi_int32*) v->value;
  if (v->type == MVAR_INT64_T) return (mi_float32*) (mi_int64*) v->value;
  if (v->type == MVAR_FLOAT64_T) return (mi_float32*) (mi_float64*) v->value;

  else if (v->type == MVAR_FLOAT32_T) return (mi_float32*) v->value;

  printf(wstr(0x01), vtstr(v->type), vtstr(MVAR_FLOAT32_T));
  return NULL;
}

static inline mi_float64* vcastf64(mvar* v)
{
  if (v->type == MVAR_INT8_T) return (mi_float64*) (mi_int8*) v->value;
  if (v->type == MVAR_INT16_T) return (mi_float64*) (mi_int16*) v->value;
  if (v->type == MVAR_INT32_T) return (mi_float64*) (mi_int32*) v->value;
  if (v->type == MVAR_INT64_T) return (mi_float64*) (mi_int64*) v->value;
  if (v->type == MVAR_FLOAT32_T) return (mi_float64*) (mi_float32*) v->value;

  else if (v->type == MVAR_FLOAT64_T) return (mi_float64*) v->value;

  printf(wstr(0x01), vtstr(v->type), vtstr(MVAR_FLOAT32_T));
  return NULL;
}

static inline mi_int8* vcasti8(mvar* v)
{
  if (v->type == MVAR_INT16_T) return (mi_int8*) (mi_int16*) v->value;
  if (v->type == MVAR_INT32_T) return (mi_int8*) (mi_int32*) v->value;
  if (v->type == MVAR_INT64_T) return (mi_int8*) (mi_int64*) v->value;
  if (v->type == MVAR_FLOAT32_T) return (mi_int8*) (mi_float32*) v->value;
  if (v->type == MVAR_FLOAT64_T) return (mi_int8*) (mi_float64*) v->value;

  else if (v->type == MVAR_INT8_T) return (mi_int8*) v->value;

  printf(wstr(0x01), vtstr(v->type), vtstr(MVAR_INT8_T));
  return NULL;
}

static inline mi_int16* vcasti16(mvar* v)
{
  if (v->type == MVAR_INT8_T) return (mi_int16*) (mi_int8*) v->value;
  if (v->type == MVAR_INT32_T) return (mi_int16*) (mi_int32*) v->value;
  if (v->type == MVAR_INT64_T) return (mi_int16*) (mi_int64*) v->value;
  if (v->type == MVAR_FLOAT32_T) return (mi_int16*) (mi_float32*) v->value;
  if (v->type == MVAR_FLOAT64_T) return (mi_int16*) (mi_float64*) v->value;

  else if (v->type == MVAR_INT16_T) return (mi_int16*) v->value;

  printf(wstr(0x01), vtstr(v->type), vtstr(MVAR_INT16_T));
  return NULL;
}

static inline mi_int8* vcasti32(mvar* v)
{
  if (v->type == MVAR_INT8_T) return (mi_int32*) (mi_int8*) v->value;
  if (v->type == MVAR_INT16_T) return (mi_int32*) (mi_int16*) v->value;
  if (v->type == MVAR_INT64_T) return (mi_int32*) (mi_int64*) v->value;
  if (v->type == MVAR_FLOAT32_T) return (mi_int32*) (mi_float32*) v->value;
  if (v->type == MVAR_FLOAT64_T) return (mi_int32*) (mi_float64*) v->value;

  else if (v->type == MVAR_INT32_T) return (mi_int32*) v->value;

  printf(wstr(0x01), vtstr(v->type), vtstr(MVAR_INT32_T));
  return NULL;
}

static inline mi_int8* vcasti64(mvar* v)
{
  if (v->type == MVAR_INT8_T) return (mi_int64*) (mi_int8*) v->value;
  if (v->type == MVAR_INT16_T) return (mi_int64*) (mi_int16*) v->value;
  if (v->type == MVAR_INT32_T) return (mi_int64*) (mi_int32*) v->value;
  if (v->type == MVAR_FLOAT32_T) return (mi_int64*) (mi_float32*) v->value;
  if (v->type == MVAR_FLOAT64_T) return (mi_int64*) (mi_float64*) v->value;

  else if (v->type == MVAR_INT64_T) return (mi_int64*) v->value;

  printf(wstr(0x01), vtstr(v->type), vtstr(MVAR_INT64_T));
  return NULL;
}

#endif
