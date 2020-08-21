#include "MakeItLexer.h"

#include "utils/Type.h"

#include <stdlib.h>
#include <stdio.h>

static token* create_token(enum token_t type, void* value)
{
  token* t = (token*) calloc(sizeof(token), 1);
  t->type = type;
  t->value = value;
  return t;
}

static void push(array* tokens, string_buffer* str)
{
  if (strcmp("true", str->str) == 0 || strcmp("false", str->str) == 0)
    array_push(tokens, create_token(TT_BOOL, strcmp("false", str->str) == 0 ? NULL : str));
  else if (str->length > 0)
    array_push(tokens, create_token(TT_NAME, (void*) string_buffer_extract(str)));
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
  return c >= 'A' && c <= 'z';
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
      enum token_t num_type = TT_INT;
      if (data[0] == 'L' || data[0] == 'l') { c = data[0]; data++; num_type = TT_LONG; }
      else if (data[0] == 'F' || data[0] == 'f') { c = data[0]; data++; num_type = TT_FLOAT; }
      else if (data[0] == 'D' || data[0] == 'd') { c = data[0]; data++; num_type = TT_DOUBLE; }

      /* check if a decimal number was assigned to a integer */
      if ((num_type == TT_INT || num_type == TT_LONG) && num_decimal)
      {
	printf(":: decimal number in integer\n.");
	return 0;
      }

      void* num = NULL;
      if (num_type == TT_INT)
      {
	num = calloc(sizeof(int), 1);
	*((int*) num) = atoi(string_buffer_extract(number_str));
      }else if (num_type == TT_LONG)
      {
	num = calloc(sizeof(long), 1);
	*((long*) num) = atol(string_buffer_extract(number_str));
      }else if (num_type == TT_FLOAT)
      {
	num = calloc(sizeof(float), 1);
	*((float*) num) = atof(string_buffer_extract(number_str));
      }else if (num_type == TT_DOUBLE)
      {
	num = calloc(sizeof(double), 1);
	*((double*) num) = atof(string_buffer_extract(number_str));
      }
      array_push(tokens, create_token(num_type, num));
      string_buffer_delete(number_str);
    }else if (c == ' ' || c == EXPR_END)
    {
      push(tokens, str);
      if (c == EXPR_END)
	array_push(tokens, create_token(TT_END, NULL));
    }else if (c == '(' || c == ')')
    {
      push(tokens, str);
      array_push(tokens, create_token(c == '(' ? TT_ARGS_BEGIN : TT_ARGS_END, NULL));
    }else if (c == '[' || c == ']')
    {
      push(tokens, str);
      if (c == '[')
	array_push(tokens, create_token(TT_AT, NULL));
      else
	array_push(tokens, create_token(TT_AT_END, NULL));
    }else if (c == ',' || c == '.')
    {
      push(tokens, str);
      array_push(tokens, create_token(c == ',' ? TT_SEPERATOR : TT_DOT, NULL));
    }else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|' || c == '~')
    {
      push(tokens, str);
      if (c == '+' && data[0] == '+') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_INCREMENT, NULL)); }
      else if (c == '-' && data[0] == '-') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_DECREMENT, NULL)); }
      else if (c == '+' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_ADDV, NULL)); }
      else if (c == '-' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_SUBV, NULL)); }
      else if (c == '*' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_MULV, NULL)); }
      else if (c == '/' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_DIVV, NULL)); }
      else if (c == '%' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_MODV, NULL)); }
      else if (c == '=' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_EQUAL, NULL)); }
      else if (c == '>' && data[0] == '>') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_BIT_RIGHT_SHIFT, NULL)); }
      else if (c == '<' && data[0] == '<') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_BIT_LEFT_SHIFT, NULL)); }
      else if (c == '&' && data[0] == '&') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_AND, NULL)); }
      else if (c == '|' && data[0] == '|') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_OR, NULL)); }
      else if (c == '!' && data[0] == '=') { c = data[0]; data++; array_push(tokens, create_token(TT_OPR_NOT_EQUAL, NULL)); }

      else if (c == '+') array_push(tokens, create_token(TT_OPR_ADD, NULL));
      else if (c == '-') array_push(tokens, create_token(TT_OPR_SUB, NULL));
      else if (c == '*') array_push(tokens, create_token(TT_OPR_MUL, NULL));
      else if (c == '/') array_push(tokens, create_token(TT_OPR_DIV, NULL));
      else if (c == '%') array_push(tokens, create_token(TT_OPR_MOD, NULL));
      else if (c == '!') array_push(tokens, create_token(TT_OPR_NOT, NULL));
      else if (c == '&') array_push(tokens, create_token(TT_OPR_BIT_AND, NULL));
      else if (c == '|') array_push(tokens, create_token(TT_OPR_BIT_OR, NULL));
      else if (c == '=') array_push(tokens, create_token(TT_OPR_ASSIGN, NULL));
      else if (c == '~') array_push(tokens, create_token(TT_OPR_BIT_XOR, NULL));
      else if (c == '>') array_push(tokens, create_token(TT_OPR_MORE, NULL));
      else if (c == '<') array_push(tokens, create_token(TT_OPR_LESS, NULL));
    }else if (c == '"')
    {
      push(tokens, str);
      while (true)
      {
	c = data[0];
	data++;

	if (c == '"' && last_c != '\\')
	  break;
	string_buffer_appendc(str, c);
	last_c = c;
      }
      array_push(tokens, create_token(TT_STRING, string_buffer_extract(str)));
      string_buffer_clear(str);
    }else if (is_name(c) || is_digit(c, false))
      string_buffer_appendc(str, c);
    last_c = c;
  }
  MILEX_proctokens(tokens);
  return 1;
}

int MILEX_proctokens(array* tokens)
{
  iterator* iter = (iterator*) calloc(sizeof(iterator), 1);
  iter_init(iter, tokens);

  script* s = (script*) calloc(sizeof(script), 1);

  s->variables = (map*) calloc(sizeof(map), 1);
  map_init(s->variables, 32);

  while (iter_has(iter))
  {
    token* t = (token*) iter_next(iter);
    if (t->type == TT_END) 
      continue;
    if (t->type == TT_NAME)
    {
      char* name = (char*) t->value;
      if (strcmp("var", name) == 0)
      {
	if (MILEX_procvar(iter, s) != 1)
	  return 0;
      }else
      {
	variable* var = (variable*) map_pull(s->variables, name);
	if (var != NULL)
	{
	  t = (token*) iter_next(iter);
	  map_push(s->variables, name, MILEX_procopr(t->type, var, iter, s));
	}
      }
    }
  }

  for (uint32_t i = 0; i < s->variables->used; i++)
  {
    variable* var = s->variables->values[i];
    printf("var[%s]: ", s->variables->keys[i]);
    if (var->type == VAR_INT_T) printf("%i\n", *((int*) var->value));
    if (var->type == VAR_FLOAT_T) printf("%f\n", *((float*) var->value));
    if (var->type == VAR_DOUBLE_T) printf("%f\n", *((double*) var->value));
    if (var->type == VAR_BOOL_T) printf("%s\n", *((uint8_t*) var->value) == 0 ? "false" : "true");
  }
  return 1;
}

int MILEX_procvar(iterator* iter, script* s)
{
  if (iter_has(iter) || ((token*) iter_peek(iter))->type == TT_NAME)
  {
    char* var_name = (char*) ((token*) iter_next(iter))->value;
    variable* var_value = (variable*) calloc(sizeof(variable), 1);
    if (iter_has(iter) && ((token*) iter_peek(iter))->type == TT_OPR_ASSIGN)
      var_value = MILEX_procopr(TT_OPR_ASSIGN, var_value, iter, s);
    map_push(s->variables, var_name, var_value);
  }else
  { 
    printf(":: variable name not specified.\n");
    return 0;
  }
  return 1;
}

variable* MILEX_procopr(enum token_t type, variable* var, iterator* iter, script* s)
{
  if (type == TT_OPR_ASSIGN)
    var->value = MILEX_procval(iter, &var->type, s);
  else if (type == TT_OPR_ADDV || type == TT_OPR_SUBV || type == TT_OPR_MULV || type == TT_OPR_DIVV || type == TT_OPR_MODV)
    var->value = MILEX_procopra(type, &var->type, var->value, MILEX_procval(iter, &var->type, s));
  return var;
}

uint8_t* MILEX_procval(iterator* iter, uint8_t* type, script* s)
{
  uint8_t* value = NULL;
  while (iter_has(iter))
  {
    if (((token*) iter_peek(iter))->type == TT_END)
      break;
    token* t = (token*) iter_next(iter);
    if (t->type == TT_INT)
    {
      value = calloc(sizeof(int), 1);
      *type = VAR_INT_T;
      *((int*) value) = *((int*) t->value);
    }else if (t->type == TT_LONG)
    {
      value = calloc(sizeof(long), 1);
      *type = VAR_LONG_T;
      *((long*) value) = *((long*) t->value);
    }else if (t->type == TT_FLOAT)
    {
      value = calloc(sizeof(float), 1);
      *type = VAR_FLOAT_T;
      *((float*) value) = *((float*) t->value);
    }else if (t->type == TT_DOUBLE)
    {
      value = calloc(sizeof(double), 1);
      *type = VAR_DOUBLE_T;
      *((double*) value) = *((double*) t->value);
    }else if (t->type == TT_OPR_ADD || t->type == TT_OPR_SUB || t->type == TT_OPR_MUL || t->type == TT_OPR_DIV || t->type == TT_OPR_MOD)
    {
      value = MILEX_procopra(t->type, type, value, MILEX_procval(iter, type, s));
    }else if (t->type == TT_NAME)
    {
      variable* var = (variable*) map_pull(s->variables, (char*) t->value);
      if (var != NULL)
      {
	*type = var->type;

	/* we need to copy the variables value if we want stuff to work and not explode */
	uint32_t size = MILEX_sizeof(var->type);
	free(value);
	value = calloc(sizeof(uint8_t), size);
	memcpy(value, var->value, size);
      }
    }
  }
  return value;
}

uint8_t* MILEX_procopra(enum token_t t, uint8_t* type, uint8_t* value, uint8_t* from)
{
  /* adding */
  if (t == TT_OPR_ADD || t == TT_OPR_ADDV)
  {
    if (*type == VAR_INT_T) *((int*) value) += *((int*) from);
    else if (*type == VAR_LONG_T) *((long*) value) += *((long*) from);
    else if (*type == VAR_FLOAT_T) *((float*) value) += *((float*) from);
    else if (*type == VAR_DOUBLE_T) *((double*) value) += *((double*) from);

  /* subtracting */
  }else if (t == TT_OPR_SUB || t == TT_OPR_SUBV)
  {
    if (*type == VAR_INT_T) *((int*) value) -= *((int*) from);
    else if (*type == VAR_LONG_T) *((long*) value) -= *((long*) from);
    else if (*type == VAR_FLOAT_T) *((float*) value) -= *((float*) from);
    else if (*type == VAR_DOUBLE_T) *((double*) value) -= *((double*) from);

  /* multiplying */
  }else if (t == TT_OPR_MUL || t == TT_OPR_MULV)
  {
    if (*type == VAR_INT_T) *((int*) value) *= *((int*) from);
    else if (*type == VAR_LONG_T) *((long*) value) *= *((long*) from);
    else if (*type == VAR_FLOAT_T) *((float*) value) *= *((float*) from);
    else if (*type == VAR_DOUBLE_T) *((double*) value) *= *((double*) from);

  /* division */
  }else if (t == TT_OPR_DIV || t == TT_OPR_DIVV)
  {
    if (*type == VAR_INT_T) *((int*) value) /= *((int*) from);
    else if (*type == VAR_LONG_T) *((long*) value) /= *((long*) from);
    else if (*type == VAR_FLOAT_T) *((float*) value) /= *((float*) from);
    else if (*type == VAR_DOUBLE_T) *((double*) value) /= *((double*) from);

  /* modulo */
  }else if (t == TT_OPR_MOD || t == TT_OPR_MODV)
  {
    if (*type == VAR_INT_T) *((int*) value) %= *((int*) from);
    else if (*type == VAR_LONG_T) *((long*) value) %= *((long*) from);
  }
  return value;
}

uint32_t MILEX_sizeof(int type)
{
  /* TODO: add more here */
  if (type == VAR_CHAR_T) return sizeof(char);
  else if (type == VAR_SHORT_T) return sizeof(short);
  else if (type == VAR_INT_T) return sizeof(int);
  else if (type == VAR_LONG_T) return sizeof(long);
  else if (type == VAR_FLOAT_T) return sizeof(float);
  else if (type == VAR_DOUBLE_T) return sizeof(double);
  else if (type == VAR_BOOL_T) return 1;
  return 0;
}
