#ifndef MI_VAR_H
  #define MI_VAR_H

#include "mtype.h"

#include "mconf.h"
#include "mtoken.h"

#include "../utils/Type.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    case MVAR_ARRAY_T: return "array"; break;
    case MVAR_RECORD_T: return "record"; break;
    case MVAR_ANY_T: return "auto"; break;
    default:
      return "?";
    break;
  }
}

static inline uint32_t vtsizeof(enum mvar_t type)
{
  /* TODO: add more here */
  if (type == MVAR_BOOL_T) return 1;
  else if (type == MVAR_INT8_T) return sizeof(char);
  else if (type == MVAR_INT16_T) return sizeof(short);
  else if (type == MVAR_INT32_T) return sizeof(int);
  else if (type == MVAR_FLOAT32_T) return sizeof(float);
  else if (type == MVAR_FLOAT64_T) return sizeof(double);
  else if (type == MVAR_STRING_T) return sizeof(char*);
  else if (type == MVAR_ARRAY_T) return sizeof(marray);
  else if (type == MVAR_RECORD_T) return sizeof(mrecord);
  return 0;
}

static inline uint32_t vsizeof(mvar* var)
{
  if (var->type == MVAR_BOOL_T)
    return vtsizeof(var->type);

  if (var->type == MVAR_STRING_T)
  {
    return strlen((char*) var->value);
  }else if (var->type == MVAR_ARRAY_T)
  {
    return ((marray*) var->value)->arr->used * vtsizeof(((marray*) var->value)->type);
  }else if (var->type == MVAR_RECORD_T)
  {
    return ((mrecord*) var->value)->size;
  }
  return 0;
}

static inline mvar* vnew(enum mvar_t type, bool constant, void* value)
{
  mvar* var = (mvar*) calloc(sizeof(mvar), 1);
  var->type = type;
  var->constant = constant;
  var->value = value;
  return var;
}

static inline void vcpy(mvar** dest, mvar* var)
{
  void* value = NULL;

  if (var->type == MVAR_BOOL_T)
    value = var->value;
  else
  {
    uint32_t size = sizeof(var->value);
    value = calloc(1, size);
    memcpy(value, var->value, size);
  }

  *dest = vnew(var->type, var->constant, value);
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
