#include "mvm.h"

#include "mfunc.h"
#include "mvar.h"
#include "mtoken.h"

#include "../utils/String.h"

int MIVM_compile(array* tokens, mscript* script)
{
  iterator* iter = iter_new(tokens);

  mscope* scope = mscope_new(script, NULL);
  if (MIVM_procscope(iter, script, scope) != 1)
    return 0;
  return 1;
}

int MIVM_procscope(iterator* iter, mscript* script, mscope* scope)
{
  while (iter_has(iter))
  {
    mtoken* token = (mtoken*) iter_peek(iter);

    if (token->type == MTK_END_T && iter_next(iter) != NULL)
      continue;

    if (token->type == MTK_SCOPE_T)
    {
      mscope* new_scope = mscope_new(script, scope);
      if (MIVM_procscope(iter_new((array*) token->value), script, new_scope) == 0)
	return 0;
    }else
    {
      mvar* var = NULL;
      if (MIVM_proctokens(iter, script, scope, &var) == 0)
	return 0;
    }
  }
  return 1;
}

int MIVM_proctokens(iterator* iter, mscript* script, mscope* scope, mvar** var)
{
  while (iter_has(iter))
  {
    mtoken* token = (mtoken*) iter_next(iter);

    if (token->type == MTK_LITERIAL_T)
    {
      mtoken* peek = (mtoken*) iter_peek(iter);

      char* var_name = (char*) token->value;

      *var = mscope_pull(scope, var_name, NULL);

      if (*var == NULL && peek != NULL && peek->type == MTK_OPERATOR_T && ((enum mopr_t) peek->value) == MOPR_ASSIGN_T)
      {
	*var = vnew(MVAR_NULL_T, false, NULL);
	map_push(scope->variables, var_name, *var);
      }else if (*var == NULL)
      {
	ferr(token->location, "undefined variable '%s'", var_name);
	return 0;
      }
    }else if (token->type == MTK_OPERATOR_T)
    {
      enum mopr_t type = (enum mopr_t) token->value;

      mvar* var2 = NULL;
      int result = MIVM_proctokens(iter, script, scope, &var2);

      if (result == 0)
	return 0;

      *var = MIVM_procoprs(type, *var, var2);

      if (result == 2)
	return 2;
    }else if (token->type == MTK_GROUP_T)
    {
      array* tokens = token->value;

      int result = MIVM_proctokens(iter_new(tokens), script, scope, var);

      if (result == 0)
	return 0;
      else if (result == 2)
	return 2;
    }else if (token->type == MTK_CALL_T)
    {
      mfunc_call* call = token->value;
      iterator* args_iter = iter_new(call->args);

      array* call_args = array_new(16);

      while (iter_has(args_iter))
      {
	mvar* var2 = NULL;

	/* ignore result (2) */
	int result = MIVM_proctokens(args_iter, script, scope, &var2);

	if (result == 0)
	  return 0;

	if (var2 == NULL)
	{
	  ferr(token->location, "expected value, found nothing.");
	  return 0;
	}

	array_push(call_args, var2);
      }

      *var = call->func->exec(script, scope, call_args);
    }else if (token->type == MTK_VALUE_T)
    {
      *var = token->value;
    }else if (token->type == MTK_END_T || token->type == MTK_COMMA_T)
    {
      return 2;
    }
  }
  return 1;
}

mvar* MIVM_procoprs(enum mopr_t opr_type, mvar* v1, mvar* v2)
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
  if (opr_type == MOPR_ART_ADD_T || opr_type == MOPR_AA_ADD_T)
  {
    if (v1->type == MVAR_STRING_T && v2->type == MVAR_STRING_T)
      v1->value = strjoin((char*) v1->value, (char*) v2->value);
    else if (v1->type == MVAR_STRING_T && v2->type == MVAR_INT8_T)
      v1->value = strjoinc((char*) v1->value, *((mi_int8*) v2->value));
    eoprcf(v1->type, +=, v1, v2);
  }

  /* subtracting */
  else if (opr_type == MOPR_ART_SUB_T || opr_type == MOPR_AA_SUB_T)
  {
    eoprcf(v1->type, -=, v1, v2);
  }

  /* multiplying */
  else if (opr_type == MOPR_ART_MUL_T || opr_type == MOPR_AA_MUL_T)
  {
    eoprcf(v1->type, *=, v1, v2);
  }

  /* division */
  else if (opr_type == MOPR_ART_DIV_T || opr_type == MOPR_AA_DIV_T)
  {
    eoprcf(v1->type, /=, v1, v2);
  }

  /* modulo */
  else if (opr_type == MOPR_ART_MOD_T || opr_type == MOPR_AA_MOD_T)
  {
    eoprc(v1->type, %=, v1, v2);
  }

  /* assign */
  else if (opr_type == MOPR_ASSIGN_T)
  {
    v1->type = v2->type;
    v1->value = v2->value;
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
  else if (opr_type == MOPR_EQL_EQUALS_T || opr_type == MOPR_EQL_NOT_EQUALS_T)
  {
    eeopr(v1->type, opr_type, MOPR_EQL_EQUALS_T, ==, v1, v2, &output);
    if (output == NULL) // works with boolean
      output = eqls(opr_type == MOPR_EQL_EQUALS_T, (v1 == v2));
  }

  /* more */
  else if (opr_type == MOPR_BOOL_MORE_T)
  {
    eeopr(v1->type, opr_type, MOPR_BOOL_MORE_T, >, v1, v2, &output);
  }

  /* less */
  else if (opr_type == MOPR_BOOL_LESS_T)
  {
    eeopr(v1->type, opr_type, MOPR_BOOL_LESS_T, <, v1, v2, &output);
  }

  if (
      opr_type == MOPR_EQL_EQUALS_T ||
      opr_type == MOPR_EQL_NOT_EQUALS_T ||
      opr_type == MOPR_BOOL_MORE_T ||
      opr_type == MOPR_BOOL_LESS_T)
    return vnew(MVAR_BOOL_T, true, output);
  return v1;
}
