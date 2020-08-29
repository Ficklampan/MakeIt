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

    if (token->type == MTK_STATEMENT_T)
    {
      token = (mtoken*) iter_next(iter);
      if (MIVM_procstatement(iter, token, script, scope) != 1)
	return 0;
    }else if (token->type == MTK_SCOPE_T)
    {
      token = (mtoken*) iter_next(iter);

      mscope* new_scope = mscope_new(script, scope);
      if (MIVM_procscope(iter_new((array*) token->value), script, new_scope) != 1)
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

static inline int checkstate(iterator* group_iter, mtoken* group_token, mscript* script, mscope* scope, bool* out)
{
  mvar* var = NULL;
  if (MIVM_proctokens(group_iter, script, scope, &var) != 1)
    return 0;

  if (var == NULL)
  {
    ferr(group_token->location, "group returned null pointer.");
    return 0;
  }

  if (var->type != MVAR_BOOL_T)
  {
    ferr(group_token->location, "group returned '%s'. expected bool.", vtstr(var->type));
    return 0;
  }

  *out = var->value == MI_BOOL_TRUE ? true : false;

  return 1;
}
int MIVM_procstatement(iterator* iter, mtoken* token, mscript* script, mscope* scope)
{
  enum mstate_t type = (enum mstate_t) token->value;
  if (type == MSTATE_IF_T || type == MSTATE_WHILE_T || type == MSTATE_FOR_T)
  {
    mtoken* group_token = (mtoken*) iter_next(iter);
    if (group_token == NULL || group_token->type != MTK_GROUP_T)
    {
      ferr(group_token == NULL ? token->location : group_token->location, "expected group. found %s.", group_token != NULL ? ttstr(group_token->type) : "nothing");
      return 0;
    }

    /* check if group is empty */
    if (((array*) group_token->value)->used == 0)
    {
      ferr(group_token->location, "group is empty.");
      return 0;
    }

    /* check scope */
    mtoken* scope_token = (mtoken*) iter_next(iter);
    if (scope_token == NULL || scope_token->type != MTK_SCOPE_T)
    {
      ferr(scope_token == NULL ? group_token->location : scope_token->location, "expected scope. found %s.", scope_token != NULL ? ttstr(scope_token->type) : "nothing");
      return 0;
    }

    mscope* state_scope = mscope_new(script, scope);
    iterator* group_iter = iter_new((array*) group_token->value);
    iterator* scope_iter = iter_new((array*) scope_token->value);

    if (type == MSTATE_IF_T)
    {
      bool state = false;
      if (checkstate(group_iter, group_token, script, scope, &state) != 1)
	return 0;

      if (state && MIVM_procscope(scope_iter, script, state_scope) != 1)
	return 0;

      /* reset the iterators (not needed here but C: ) */
      iter_reset(group_iter);
      iter_reset(scope_iter);
    }else if (type == MSTATE_WHILE_T)
    {
      while (true)
      {
	bool state = false;
	if (checkstate(group_iter, group_token, script, scope, &state) != 1)
	  return 0;

	if (!state)
	  break;

	if (MIVM_procscope(scope_iter, script, state_scope) != 1)
	  return 0;

	/* reset the iterators so we can use them again */
	iter_reset(group_iter);
	iter_reset(scope_iter);
      }
    }
  }

  return 1;
}


int MIVM_proctokens(iterator* iter, mscript* script, mscope* scope, mvar** var)
{
  while (iter_has(iter))
  {
    mtoken* token = (mtoken*) iter_next(iter);
    if (token == NULL || token->type == MTK_END_T || token->type == MTK_COMMA_T)
      return 2;

    mtoken* next = (mtoken*) iter_peek(iter);

    /* check if token is type 'operator', do stuff */
    if (token->type == MTK_OPERATOR_T)
    {
      if (next == NULL || (next->type != MTK_VALUE_T && next->type != MTK_GROUP_T && next->type != MTK_LITERIAL_T && next->type != MTK_CALL_T))
      {
	ferr(next->location, "expected value after operator.");
	return 0;
      }
      mvar* var2 = NULL;

      int result = MIVM_proctokens(iter, script, scope, &var2);
      if (result == 0)
	return 0;

      if (var2 == NULL || var2->type == MVAR_NULL_T)
      {
	ferr(next->location, "variable returned null pointer.");
	return 0;
      }
      *var = MIVM_procoprs((enum mopr_t) token->value, *var, var2);

      if (result == 2)
	return 1;
    }

    /* check if token is type 'value', return value */
    else if (token->type == MTK_VALUE_T)
    {
      vcpy(var, (mvar*) token->value);
    }

    /* if type 'group' then evaluate everything in the group */
    else if (token->type == MTK_GROUP_T)
    {
      array* tokens = (array*) token->value;
      if (tokens == NULL)
      {
	ferr(token->location, "group returned null pointer.");
	return 0;
      }else if (tokens->used == 0)
      {
	fwarn(token->location, "group does not contain any value.");
      }

      int result = MIVM_proctokens(iter_new(tokens), script, scope, var);
      if (result == 0)
	return 0;
      else if (result == 2)
	return 1;
    }

    /* check if variable */
    else if (token->type == MTK_LITERIAL_T)
    {
      mscope* src_scope = NULL;
      *var = mscope_pull(scope, (char*) token->value, &src_scope);
      bool assign_next = next != NULL && next->type == MTK_OPERATOR_T && ((enum mopr_t) next->value) == MOPR_ASSIGN_T;

      if (*var == NULL && !assign_next)
      {
	ferr(token->location, "undefined variable '%s'.", (char*) token->value);
	return 0;
      }else if (*var == NULL && assign_next)
      {
	*var = vnew(MVAR_VOID_T, false, NULL);
	map_push(src_scope->variables, (char*) token->value, *var);
      }
    }

    /* if type 'call' then return function returned value */
    else if (token->type == MTK_CALL_T)
    {
      mfunc_call* call = (mfunc_call*) token->value;

      MIVM_procfunc(call, script, scope, var);
    }

  }
  return 1;
}

int MIVM_procfunc(mfunc_call* func_call, mscript* script, mscope* scope, mvar** var)
{
  array* args = array_new(16);
  iterator* iter = iter_new(func_call->args);

  while (iter_has(iter))
  {
    mtoken* t = (mtoken*) iter_peek(iter);
    if (t == NULL)
      break;

    mvar* var1 = NULL;
    if (MIVM_proctokens(iter, script, scope, &var1) != 1)
      return 0;
    if (var1 == NULL)
    {
      ferr(t->location, "variable returned null pointer.");
      return 0;
    }

    array_push(args, var1);
  }

  // TODO: error checking
  
  *var = func_call->func->exec(args);

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
