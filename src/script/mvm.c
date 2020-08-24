#include "mvm.h"

#include "../utils/String.h"

#include "mfunc.h"
#include "mvar.h"
#include "mtoken.h"

mvar* MILEX_procfunc(mfunc_call* func_call, mscript* script, mscope* scope)
{
  array* args = array_new(16);
  array* arg = array_new(16);
  array* tokens = func_call->scope;

  iterator* iter = iter_new(tokens);
  while (iter_has(iter))
  {
    mtoken* t = (mtoken*) iter_next(iter);
    if (t->type == MTK_COMMA_T)
    {
      array_push(args, MILEX_procvals(iter_new(arg), script, scope));
      array_clear(arg);
      continue;
    }
    array_push(arg, t);
  }
  array_push(args, MILEX_procvals(iter_new(arg), script, scope));

  /* check if arg count equals function arg count */
  if (args->used < func_call->func->argc)
  {
    printf(":: too few args, expected %i but found %i.\n", func_call->func->argc, args->used);
    return NULL;
  }else if (args->used > func_call->func->argc)
  {
    printf(":: too many args, expected %i but found %i.\n", func_call->func->argc, args->used);
    return NULL;
  }

  /* check if args matches function args type */
  for (uint32_t i = 0; i < args->used; i++)
  {
    mvar* var = (mvar*) args->values[i];
    enum mvar_t var_type = func_call->func->args[i];
    if (var_type == MVAR_ANY_T)
      continue;
    if (var->type != var_type)
    {
      printf(":: variable type not match function args type, at arg[%i].\n", i);
      return NULL;
    }
  }
  return func_call->func->exec(args);
}

mvar* MILEX_procvals(iterator* iter, mscript* script, mscope* scope)
{
  mvar* var = vnew(MVAR_VOID_T, true, NULL);
  while (iter_has(iter))
  {
    mtoken* peek = (mtoken*) iter_peek(iter);
    if (peek->type == MTK_END_T)
      break;
    mtoken* t = (mtoken*) iter_next(iter);

    if (t->type == MTK_NULL_T || t->type == MTK_BOOL_T || t->type == MTK_CHAR_T || t->type == MTK_INT_T || t->type == MTK_LONG_T || t->type == MTK_FLOAT_T || t->type == MTK_DOUBLE_T || t->type == MTK_STRING_T)
     vtoken(var, t);
    else if (t->type == MTK_OPR_ASSIGN_T)
      vcpy(var, MILEX_procvals(iter, script, scope));
    else if (t->type == MTK_OPR_ADD_VAL_T || t->type == MTK_OPR_SUB_VAL_T || t->type == MTK_OPR_MUL_VAL_T || t->type == MTK_OPR_DIV_VAL_T || t->type == MTK_OPR_MOD_VAL_T || t->type == MTK_OPR_EQUAL_T || t->type == MTK_OPR_NOT_EQUAL_T || t->type == MTK_OPR_MORE_T || t->type == MTK_OPR_LESS_T)
    {
      mvar* var1 = MILEX_procvals(iter, script, scope);
      var = MILEX_procoprs(t->type, var, var1);
    }
    else if (t->type == MTK_OPR_ADD_T || t->type == MTK_OPR_SUB_T || t->type == MTK_OPR_MUL_T || t->type == MTK_OPR_DIV_T || t->type == MTK_OPR_MOD_T)
    {
      mvar* var1 = MILEX_procvals(iter, script, scope);
      var = MILEX_procoprs(t->type, var, var1);
    }
    else if (t->type == MTK_LITERIAL_T)
    {
      var = (mvar*) map_pull(scope->variables, (char*) t->value);

      if (var == NULL)
	var = vnew(MVAR_VOID_T, false, NULL);
      map_push(scope->variables, (char*) t->value, var);
    }else if (t->type == MTK_FUNC_CALL_T)
    {
      mfunc_call* func_call = (mfunc_call*) t->value;

      var = vcpy(var, MILEX_procfunc(func_call, script, scope));
    }else if (t->type == MTK_PAR_T)
    {
      array* par = (array*) t->value;
      var = MILEX_procvals(iter_new(par), script, scope);
    }
  }
  return var;
}

mvar* MILEX_procoprs(enum mtoken_t opr_type, mvar* v1, mvar* v2)
{
  uint8_t* output = NULL;

#define eopr(vt, t, op, v1, v2) { if (vt == t) nopr(op, *v1, *v2); }
#define eoprc(t, op, v1, v2) { \
			  eopr(t, MVAR_INT8_T, op, vcasti8(v1), vcasti8(v2)); \
			  eopr(t, MVAR_INT16_T, op, vcasti16(v1), vcasti16(v2)); \
    			  eopr(t, MVAR_INT32_T, op, vcasti32(v1), vcasti32(v2)); \
    			  eopr(t, MVAR_INT64_T, op, vcasti64(v1), vcasti64(v2)); \
			}
#define eoprcf(t, op, v1, v2) { \
			  eoprc(t, op, v1, v2); \
    			  eopr(t, MVAR_FLOAT32_T, op, vcastf32(v1), vcastf32(v2)); \
    			  eopr(t, MVAR_FLOAT64_T, op, vcastf64(v1), vcastf64(v2)); \
			}

  /* adding */
  if (opr_type == MTK_OPR_ADD_T || opr_type == MTK_OPR_ADD_VAL_T)
  {
    if (v1->type == MVAR_STRING_T && (v2->type == MVAR_STRING_T || v2->type == MVAR_INT8_T))
      v1->value = (uint8_t*) strjoin((char*) v1->value, (char*) v2->value);
    eoprcf(v1->type, +=, v1, v2);
  }

  /* subtracting */
  else if (opr_type == MTK_OPR_SUB_T || opr_type == MTK_OPR_SUB_VAL_T)
  {
    eoprcf(v1->type, -=, v1, v2);
  }

  /* multiplying */
  else if (opr_type == MTK_OPR_MUL_T || opr_type == MTK_OPR_MUL_VAL_T)
  {
    eoprcf(v1->type, *=, v1, v2);
  }

  /* division */
  else if (opr_type == MTK_OPR_DIV_T || opr_type == MTK_OPR_DIV_VAL_T)
  {
    eoprcf(v1->type, /=, v1, v2);
  }

  /* modulo */
  else if (opr_type == MTK_OPR_MOD_T || opr_type == MTK_OPR_MOD_VAL_T)
  {
    eoprc(v1->type, %=, v1, v2);
  }

/* -- BOOLEANS -- */
#define eqls(t, v) (t ? v : !v) ? MI_BOOL_TRUE : MI_BOOL_FALSE
#define eeopr(vt, vot, ot, op, v1, v2, output) { \
				    if (vt == MVAR_FLOAT32_T) *output = eqls(vot == ot, nopr(op, *vcastf32(v1), *vcastf32(v2))); \
				    else if (vt == MVAR_FLOAT64_T) *output = eqls(vot == ot, nopr(op, *vcastf64(v1), *vcastf64(v2))); \
    				    else if (vt == MVAR_INT8_T) *output = eqls(vot == ot, nopr(op, *vcasti8(v1), *vcasti8(v2))); \
    				    else if (vt == MVAR_INT16_T) *output = eqls(vot == ot, nopr(op, *vcasti16(v1), *vcasti16(v2))); \
    				    else if (vt == MVAR_INT32_T) *output = eqls(vot == ot, nopr(op, *vcasti32(v1), *vcasti32(v2))); \
    				    else if (vt == MVAR_INT64_T) *output = eqls(vot == ot, nopr(op, *vcasti64(v1), *vcasti64(v2))); \
			      }

  /* equals */
  else if (opr_type == MTK_OPR_EQUAL_T || opr_type == MTK_OPR_NOT_EQUAL_T)
  {
    eeopr(v1->type, opr_type, MTK_OPR_EQUAL_T, ==, v1, v2, &output);
    if (output == NULL) // works with boolean
      output = eqls(opr_type == MTK_OPR_EQUAL_T, (v1 == v2));
  }

  /* more */
  else if (opr_type == MTK_OPR_MORE_T)
  {
    eeopr(v1->type, opr_type, MTK_OPR_MORE_T, >, v1, v2, &output);
  }

  /* less */
  else if (opr_type == MTK_OPR_LESS_T)
  {
    eeopr(v1->type, opr_type, MTK_OPR_LESS_T, <, v1, v2, &output);
  }

  if (
      opr_type == MTK_OPR_EQUAL_T ||
      opr_type == MTK_OPR_NOT_EQUAL_T ||
      opr_type == MTK_OPR_MORE_T ||
      opr_type == MTK_OPR_LESS_T)
    return vnew(MVAR_BOOL_T, true, output);
  return v1;
}
