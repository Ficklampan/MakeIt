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

      array_push(tokens, create_token(num_type, string_buffer_extract(number_str)));
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
    }else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=')
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

  for (uint32_t i = 0; i < tokens->used; i++)
  {
    token* t = (token*) tokens->values[i];
    if (t->type == TT_END)
      printf("[END]\n");
    if (t->type == TT_NAME)
      printf("%s\n", (char*) t->value);
    else if (t->type == TT_STRING)printf("string: %s\n", (char*) t->value);
    else if (t->type == TT_INT)printf("int: %s\n", (char*) t->value);
    else if (t->type == TT_LONG)printf("long: %s\n", (char*) t->value);
    else if (t->type == TT_FLOAT)printf("float: %s\n", (char*) t->value);
    else if (t->type == TT_DOUBLE)printf("double: %s\n", (char*) t->value);
    else if (t->type == TT_BOOL)printf("bool: %s\n", t->value == NULL ? "false" : "true");
    else if (t->type == TT_OPR_ADD) printf("[add]\n");
    else if (t->type == TT_OPR_SUB) printf("[sub]\n");
    else if (t->type == TT_OPR_MORE) printf("[more]\n");
    else if (t->type == TT_OPR_LESS) printf("[less]\n");
    else if (t->type == TT_OPR_ASSIGN) printf("[assign]\n");
    else if (t->type == TT_OPR_ADDV) printf("[add to]\n");
    else if (t->type == TT_OPR_INCREMENT) printf("[increment]\n");
    else if (t->type == TT_ARGS_BEGIN) printf("[args start]\n");
    else if (t->type == TT_ARGS_END) printf("[args end]\n");
    else if (t->type == TT_SEPERATOR) printf("[seperate]\n");
    else if (t->type == TT_DOT) printf("[dot]\n");
    else if (t->type == TT_AT) printf("[at]\n");
    else if (t->type == TT_AT_END) printf("[at end]\n");
  }
  return 1;
}

int MILEX_procdat(array* tokens, string_buffer* compiled)
{
  iterator* iter = (iterator*) calloc(sizeof(iterator), 1);
  iter_init(iter, tokens);

  map* variables = (map*) calloc(sizeof(map), 1);
  map_init(variables, 32);

  while (iter_has(iter))
  {
    char* token = (char*) iter_next(iter);
    if (token[0] == ';')
      continue;
    if (strcmp("var", token) == 0)
    {
      if (iter_has(iter) && ((char*) iter_peek(iter))[0] != ';')
      {
	char* var_name = (char*) iter_next(iter);
	char* var_value = "";
	if (iter_has(iter) && ((char*) iter_peek(iter))[0] == '=')
	{
	  if (iter_has(iter) && ((char*) iter_peek(iter))[0] != ';')
	    var_value = (char*) iter_next(iter);
	  else
	  {
	    printf(":: no variable value specified.\n");
	    return 0;
	  }
	}
	map_push(variables, var_name, var_value);
      }else
      {
	printf(":: no variable name specified.\n");
	return 0;
      }
    }else
    {
      if (map_pull(variables, token) == NULL)
      {
	printf(":: undefined variable '%s'.\n", token);
	return 0;
      }
      if (iter_has(iter) && ((char*) iter_peek(iter))[0] != ';')
      {
	char* var_action = (char*) iter_next(iter);
	if (strcmp(var_action, "+=") == 0 || strcmp(var_action, "=") == 0)
	{
	  if (iter_has(iter) && ((char*) iter_peek(iter))[0] != ';')
	  {
	    string_buffer_appendc(compiled, strcmp(var_action, "+=") == 0 ? '+' : '=');
	    string_buffer_append(compiled, token);
	    string_buffer_appendc(compiled, '\0');

	    while (true)
	    {
	      if (iter_has(iter) && ((char*) iter_peek(iter))[0] != ';')
	      {
		string_buffer_append(compiled, (char*) iter_next(iter));
		string_buffer_appendc(compiled, '\0');
		continue;
	      }
	      break;
	    }
	  }else
	  {
	    printf(":: no value specified.\n");
	    return 0;
	  }
	}else
	{
	  printf(":: unknown action '%s'.\n", var_action);
	  return 0;
	}
      }else
      {
	printf(":: nothing to do with variable '%s'.\n", token);
	return 0;
      }
    }
  }
  return 1;
}
