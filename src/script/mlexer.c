#include "mlexer.h"

#include "mvm.h"

#include "../utils/String.h"

#include <stdlib.h>
#include <stdio.h>

static mtoken* create_token(enum mtoken_t type, void* value)
{
  mtoken* t = (mtoken*) calloc(sizeof(mtoken), 1);
  t->type = type;
  t->value = value;
  return t;
}

static void push(array* tokens, string_buffer* str)
{
  if (strcmp("null", str->str) == 0)
    array_push(tokens, create_token(MTK_NULL_T, NULL));
  else if (strcmp("true", str->str) == 0 || strcmp("false", str->str) == 0)
    array_push(tokens, create_token(MTK_BOOL_T, strcmp("true", str->str) == 0 ? MI_BOOL_TRUE : MI_BOOL_FALSE));
  else if (str->length > 0)
    array_push(tokens, create_token(MTK_LITERIAL_T, (void*) string_buffer_extract(str)));
  string_buffer_clear(str);
}

static inline bool is_digit(char c, bool ds)
{
  if (c >= '0' && c <= '9')
    return true;
  return c == DECIMAL_SEPERATOR && ds;
}

static inline bool is_name(char c)
{
  return (c >= 'A' && c <= 'z') || c == '_';
}

int MILEX_prsdat(const char* data, uint32_t data_length, array* tokens)
{
  string_buffer* str = (string_buffer*) calloc(sizeof(string_buffer), 1);
  string_buffer_init(str, 32);

  char last_c = '\0';
  while (data[0] != '\0')
  {
    char c = data[0];
    data++;

    if (is_digit(c, false) && str->length == 0)
    {
      /* init number string */
      string_buffer* number_str = (string_buffer*) calloc(sizeof(string_buffer), 1);
      string_buffer_init(number_str, 32);

      bool num_decimal = false;

      /* append the first digit */
      string_buffer_appendc(number_str, c);

      /* append all digits after the first */
      while (is_digit(data[0], true))
      {
	c = data[0];
	data++;

	/* check if '.' and if next char is not a digit */
	if (c == DECIMAL_SEPERATOR && (data[0] < '0' || data[0] > '9'))
	{
	  printf(":: missing digit after decimal seperator.\n");
	  return 0;
	}else if (c == DECIMAL_SEPERATOR)
	  num_decimal = true;
	string_buffer_appendc(number_str, c);
	last_c = c;
      }
      /* check number type */
      enum mtoken_t num_type = MTK_INT_T;
      if (data[0] == 'L' || data[0] == 'l') { c = data[0]; data++; num_type = MTK_LONG_T; }
      else if (data[0] == 'F' || data[0] == 'f') { c = data[0]; data++; num_type = MTK_FLOAT_T; }
      else if (data[0] == 'D' || data[0] == 'd') { c = data[0]; data++; num_type = MTK_DOUBLE_T; }

      /* check if a decimal number was assigned to a integer */
      if ((num_type == MTK_INT_T || num_type == MTK_LONG_T) && num_decimal)
      {
	printf(":: decimal number in integer\n.");
	return 0;
      }

      void* num = NULL;
      if (num_type == MTK_INT_T)
      {
	num = calloc(sizeof(int), 1);
	*((int*) num) = atoi(string_buffer_extract(number_str));
      }else if (num_type == MTK_LONG_T)
      {
	num = calloc(sizeof(long), 1);
	*((long*) num) = atol(string_buffer_extract(number_str));
      }else if (num_type == MTK_FLOAT_T)
      {
	num = calloc(sizeof(float), 1);
	*((float*) num) = atof(string_buffer_extract(number_str));
      }else if (num_type == MTK_DOUBLE_T)
      {
	num = calloc(sizeof(double), 1);
	*((double*) num) = atof(string_buffer_extract(number_str));
      }
      array_push(tokens, create_token(num_type, num));
      string_buffer_delete(number_str);
    }else if (c == ' ' || c == ';')
    {
      push(tokens, str);
      if (c == ';')
	array_push(tokens, create_token(MTK_END_T, NULL));
    }else if (c == '(' || c == ')')
    {
      push(tokens, str);
      array_push(tokens, create_token(c == '(' ? MTK_PAR_BEGIN_T : MTK_PAR_END_T, NULL));
    }else if (c == '{' || c == '}')
    {
      push(tokens, str);
      array_push(tokens, create_token(c == '{' ? MTK_SCOPE_BEGIN_T : MTK_SCOPE_END_T, NULL));
    }else if (c == '[' || c == ']')
    {
      push(tokens, str);
      if (c == '[')
	array_push(tokens, create_token(MTK_AT_BEGIN_T, NULL));
      else
	array_push(tokens, create_token(MTK_AT_END_T, NULL));
    }else if (c == ',' || c == '.')
    {
      push(tokens, str);
      array_push(tokens, create_token(c == ',' ? MTK_COMMA_T : MTK_DOT_T, NULL));
    }else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|' || c == '~')
    {
      push(tokens, str);
      if (c == '+' && data[0] == '+') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_INCREMENT_T, NULL)); }
      else if (c == '-' && data[0] == '-') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_DECREMENT_T, NULL)); }
      else if (c == '+' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_ADD_VAL_T, NULL)); }
      else if (c == '-' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_SUB_VAL_T, NULL)); }
      else if (c == '*' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_MUL_VAL_T, NULL)); }
      else if (c == '/' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_DIV_VAL_T, NULL)); }
      else if (c == '%' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_MOD_VAL_T, NULL)); }
      else if (c == '=' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_EQUAL_T, NULL)); }
      else if (c == '>' && data[0] == '>') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_BIT_RIGHT_SHIFT_T, NULL)); }
      else if (c == '<' && data[0] == '<') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_BIT_LEFT_SHIFT_T, NULL)); }
      else if (c == '&' && data[0] == '&') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_AND_T, NULL)); }
      else if (c == '|' && data[0] == '|') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_OR_T, NULL)); }
      else if (c == '!' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(MTK_OPR_NOT_EQUAL_T, NULL)); }

      else if (c == '+') array_push(tokens, create_token(MTK_OPR_ADD_T, NULL));
      else if (c == '-') array_push(tokens, create_token(MTK_OPR_SUB_T, NULL));
      else if (c == '*') array_push(tokens, create_token(MTK_OPR_MUL_T, NULL));
      else if (c == '/') array_push(tokens, create_token(MTK_OPR_DIV_T, NULL));
      else if (c == '%') array_push(tokens, create_token(MTK_OPR_MOD_T, NULL));
      else if (c == '!') array_push(tokens, create_token(MTK_OPR_NOT_T, NULL));
      else if (c == '&') array_push(tokens, create_token(MTK_OPR_BIT_AND_T, NULL));
      else if (c == '|') array_push(tokens, create_token(MTK_OPR_BIT_OR_T, NULL));
      else if (c == '=') array_push(tokens, create_token(MTK_OPR_ASSIGN_T, NULL));
      else if (c == '~') array_push(tokens, create_token(MTK_OPR_BIT_XOR_T, NULL));
      else if (c == '>') array_push(tokens, create_token(MTK_OPR_MORE_T, NULL));
      else if (c == '<') array_push(tokens, create_token(MTK_OPR_LESS_T, NULL));
    }else if (c == '"')
    {
      push(tokens, str);
      while (true)
      {
	c = data[0];
	data++;

	if (c == '"' && last_c != '\\')
	  break;
	if (last_c == '\\')
	{
	  switch (c)
	  {
	    case 'n': c = '\n'; break;
	  }
	}
	if (c != '\\' || last_c == '\\')
	  string_buffer_appendc(str, c);
	last_c = c;
      }
      array_push(tokens, create_token(MTK_STRING_T, string_buffer_extract(str)));
      string_buffer_clear(str);
    }else if (is_name(c) || is_digit(c, false))
      string_buffer_appendc(str, c);
    last_c = c; 
  }

  iterator* iter1 = iter_new(tokens);
  array* fixed_tokens1 = array_new(128);
  MILEX_packtokens(iter1, MTK_PAR_BEGIN_T, MTK_PAR_END_T, MTK_PAR_T, true, fixed_tokens1);

  iterator* iter2 = iter_new(fixed_tokens1);
  array* fixed_tokens2 = array_new(128);
  MILEX_packtokens(iter2, MTK_SCOPE_BEGIN_T, MTK_SCOPE_END_T, MTK_SCOPE_T, true, fixed_tokens2);

  mscript* script = (mscript*) calloc(sizeof(mscript), 1);
  mscope* scope = (mscope*) calloc(sizeof(mscope), 1);

  scope->variables = map_new(32);
  script->functions = map_new(32);

  func_defaults(script->functions);

  MILEX_proctokens(iter_new(fixed_tokens2), script, scope);
  return 1;
}

int MILEX_packtokens(iterator* iter, enum mtoken_t begin, enum mtoken_t end, enum mtoken_t type, bool c, array* tokens)
{
  while (iter_has(iter))
  {
    mtoken* t = (mtoken*) iter_next(iter);
    if (t->type == begin)
    {
      array* pack = array_new(16);
      if (MILEX_packtokens(iter, begin, end, type, false, pack) != 1)
	return 0;
      array_push(tokens, create_token(type, pack));
    }else if (t->type == end && !c)
      break;
    else
      array_push(tokens, t);
  }
  return 1;
}

int MILEX_proctokens(iterator* iter, mscript* script, mscope* scope)
{
  while (iter_has(iter))
  {
    mtoken* t = (mtoken*) iter_next(iter);
    if (t->type == MTK_END_T) 
      continue;

    if (t->type == MTK_LITERIAL_T)
    {
      char* name = (char*) t->value;

      if (strcmp("if", name) == 0)
      {
      }else if (strcmp("while", name) == 0)
      {
      }else if (strcmp("for", name) == 0)
      {

      }else
      {
	iter->index--;
	MILEX_procvals(iter, script, scope);
      }
    }
  }
  return 1;
}


