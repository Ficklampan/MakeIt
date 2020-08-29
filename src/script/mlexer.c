#include "mlexer.h" 
#include "mvar.h"
#include "mvm.h"

#include "../utils/String.h"

#include <stdlib.h>
#include <stdio.h>

static inline int esc_char(char c, char* o)
{
  switch (c)
  {
    case 'n': *o = '\n'; break;
    case '0': *o = '\0'; break;
    default:
      return 0;
      break;
  }
  return 1;
}

static inline bool is_blank(char c)
{
  return c == ' ' || c == '\t' || c == '\n';
}

static inline bool is_digit(char c, bool ds)
{
  if (c >= '0' && c <= '9')
    return true;
  return c == DECIMAL_SEPERATOR && ds;
}

static inline bool is_hex(char  c)
{
  return is_digit(c, false) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

static inline bool is_opr(char c)
{
  return 
    c == '+' ||
    c == '-' ||
    c == '*' ||
    c == '/' ||
    c == '%' ||
    c == '&' ||
    c == '|' ||
    c == '!' ||
    c == '=' ||
    c == '>' ||
    c == '<' ||
    c == '^' ||
    c == '~';
}

static inline bool is_name(char c)
{
  return (c >= 'A' && c <= 'z') || c == '_';
}

int MILEX_makescript(mscript* script, const char* libs)
{
  // TODO: libs
  script->functions = map_new(32);

  func_defaults(script->functions);
  return 1;
}

int MILEX_maketokens(const char* data, uint32_t len, array* tokens, const char* file, mscript* script)
{
  uint32_t index = 0;
  uint32_t lpos = 1;
  uint32_t cpos = 0;

  mtoken* lt = NULL;
  while (data[index] != '\0' && index < len)
  {
    mtoken* t = NULL;
    uint32_t start = index;

    if (MILEX_nexttoken(data, &index, &lpos, &cpos, len, file, script, lt, &t) != 1)
      return 0;

    if (t != NULL)
    {
      lt = t;
      array_push(tokens, t);
    }
    
  }
  return 1;
}

static inline char next_char(const char* data, uint32_t* index, uint32_t* lpos, uint32_t* cpos)
{
  char c = data[*index];
  *index+=1;
  if (c == '\n')
  {
    *lpos+=1;
    *cpos = 0;
  }else
    *cpos+=1;
  return c;
}

/* make better token.cpos */
int MILEX_nexttoken(const char* data, uint32_t* index, uint32_t* lpos, uint32_t* cpos, uint32_t len, const char* file, mscript* script, mtoken* last_token, mtoken** token)
{
  uint32_t start = *index;
  uint32_t startl = *lpos;
  uint32_t startc = *cpos;

#define nextc() (next_char(data, index, lpos, cpos))
#define peekc() (*index < len ? data[*index] : '\0')
#define hasc() (*index < len)
#define mktoken(t, v) (ntoken(t, v, mtloc_new(*lpos, *cpos, start, *index, (char*) file, (char*) data, len)))
#define mkferr(s) (ferr(mtloc_new(startl, startc, start, *index, (char*) file, (char*) data, len), s))
#define mkfwarn(s) (fwarn(mtloc_new(startl, startc, start, *index, (char*) file, (char*) data, len), s))

  char c = nextc();

  /* number token */
  if (is_digit(c, false))
  {
    uint32_t nlen = 1;
    bool ds = false;
    bool hex = false;

    // TODO: hex
    if (c == '0' && (peekc() == 'x' || peekc() == 'X') && nextc() != '\0')
      hex = true;

    while (true)
    {
      c = peekc();

      if (c == DECIMAL_SEPERATOR && !is_digit(peekc(), false))
      {
	mkferr("can't use a '.' here:");
	return 0;
      }else if (!is_digit(c, true))
	break;

      if (c == DECIMAL_SEPERATOR && !ds)
	ds = true;
      else if (c == DECIMAL_SEPERATOR && ds)
      {
	mkferr("multiple decimal seperators found.");
	return 0;
      }
      nextc();
      nlen++;
    }
    char nstr[nlen];
    memcpy(nstr, &data[*index - nlen], nlen);

    enum mvar_t type = MVAR_INT32_T;
    if (c == 'L') type = MVAR_INT64_T;
    else if (c == 'F') type = MVAR_FLOAT32_T;
    else if (c == 'D' || ds) type = MVAR_FLOAT64_T;

    if (ds && (type == MVAR_INT32_T || type == MVAR_INT64_T))
    {
      mkferr("cannot convert floating-point number to integer.");
      return 0;
    }

    char* number = calloc(type == MVAR_INT64_T || type == MVAR_FLOAT64_T ? 8 : 4, 1);

    if (type == MVAR_INT32_T) *number = (int) strtol(nstr, NULL, hex ? 16 : 10);
    else if (type == MVAR_INT64_T) *number = (long) strtol(nstr, NULL, hex ? 16 : 10);
    else if (type == MVAR_FLOAT32_T) *number = (float) strtof(nstr, NULL);
    else if (type == MVAR_FLOAT64_T) *number = (double) strtod(nstr, NULL);

    *token = mktoken(MTK_VALUE_T, vnew(type, false, number));
    return 1;

  /* string token */
  }else if (c == '"')
  {
    string_buffer* str_buff = string_buffer_new(32);

    bool ended = false;
    uint32_t slen = 0;
    char lc = '\0';
    while (true)
    {
      c = nextc();

      /* check if escape character */
      if (c == '\\')
      {
	c = nextc();
	if (esc_char(c, &c) != 1)
	{
	  mkferr("unknown escape character '%c'.");
	  return 0;
	}

      /* check if string has ended */
      }else if (c == '"')
      {
	ended = true;
	break;
      }

      string_buffer_appendc(str_buff, c);
      slen++;
      lc = c;
    }

    /* check if string ended with '"' */
    if (!ended)
    {
      mkferr("'\"' not ended. expected '\"' here:");
      return 0;
    }

    /* check if string is larger than max string length */
    if (slen > MAX_STRING_LENGTH)
    {
      // TODO: print the MAX_STRING_LENGTH
      mkferr("string is larger than 'MAX_STRING_LENGTH'.");
      return 0;
    }

    *token = mktoken(MTK_VALUE_T, vnew(MVAR_STRING_T, false, string_buffer_extractd(str_buff)));
    return 1;

  /* char token */
  }else if (c == '\'')
  {
    c = nextc();

    /* check if escape character */
    if (c == '\\')
    {
      c = nextc();
      if (esc_char(c, &c) != 1)
      {
	mkferr("unknown escape character '%c'.");
	return 0;
      }
    }

    /* check if ends with ' */
    if (nextc() != '\'')
    {
      mkferr("missing ' at the end. here:");
      return 0;
    }

    /* store the value on the heap */
    char* value = calloc(sizeof(char), 1);
    *value = c;

    *token = mktoken(MTK_VALUE_T, vnew(MVAR_INT8_T, false, value));
    return 1;

  /* literial / call / boolean / null value token */
  }else if (is_name(c))
  {
    string_buffer* str_buff = string_buffer_new(32);
    string_buffer_appendc(str_buff, c);

    uint32_t llen = 1;
    while (hasc())
    {
      c = peekc();

      if (!is_name(c) && !is_digit(c, false))
	break;

      string_buffer_appendc(str_buff, nextc());
    }

    /* get the string and delete the string buffer */
    char* lit = string_buffer_extractd(str_buff);

    /* check if it matches a boolean */
    if (strcmp("true", lit) == 0 || strcmp("false", lit) == 0)
      *token = mktoken(MTK_VALUE_T, vnew(MVAR_BOOL_T, false, strcmp("true", lit) == 0 ? MI_BOOL_TRUE : MI_BOOL_FALSE));

    /* check if null value */
    else if (strcmp("null", lit) == 0)
      *token = mktoken(MTK_VALUE_T, vnew(MVAR_NULL_T, false, NULL));

    /* check if statement */
    else if (strcmp("if", lit) == 0) *token = mktoken(MTK_STATEMENT_T, (void*) MSTATE_IF_T);
    else if (strcmp("else", lit) == 0) *token = mktoken(MTK_STATEMENT_T, (void*) MSTATE_ELSE_T);
    else if (strcmp("while", lit) == 0) *token = mktoken(MTK_STATEMENT_T, (void*) MSTATE_WHILE_T);

    else
    {

      // TODO: declare a function
      /* check if function / function call */
      bool next_group = false;
      for (uint32_t i = *index; i < len; i++)
      {
	if (is_blank(data[i]))
	  continue;
	if (data[i] == '(')
	  next_group = true;
	break;
      }

      if (next_group)
      {
	mtoken* group_token = NULL;
	if (MILEX_nexttoken(data, index, lpos, cpos, len, file, script, last_token, &group_token) != 1)
	  return 0;

	/* error checking */
	if (group_token == NULL)
	{
	  mkferr("next token returned null pointer, expected group.");
	  return 0;
	}else if (group_token->type != MTK_GROUP_T)
	{
	  mkferr("next token returned unexpected value.");
	  return 0;
	}

	mfunc* func = map_pull(script->functions, lit);

	/* check if function is defined */
	if (func == NULL)
	{
	  mkferr("undefined function '%s'.");
	  return 0;
	}

	mfunc_call* func_call = (mfunc_call*) calloc(sizeof(mfunc_call), 1);
	func_call->args = (array*) group_token->value;
	func_call->func = func;
	*token = mktoken(MTK_CALL_T, func_call);

	/* don't need this anymoreee */
	free(lit);

      }else
	*token = mktoken(MTK_LITERIAL_T, lit);
    }

    return 1;

  /* operator token */
  }else if (is_opr(c))
  {
    if (c == '+' && peekc() == '+' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_INCREMENT_T);
    else if (c == '-' && peekc() == '-' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_DECREMENT_T);
    else if (c == '+' && peekc() == '=' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_AA_ADD_T);
    else if (c == '-' && peekc() == '=' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_AA_SUB_T);
    else if (c == '*' && peekc() == '=' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_AA_MUL_T);
    else if (c == '/' && peekc() == '=' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_AA_DIV_T);
    else if (c == '%' && peekc() == '=' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_AA_MOD_T);
    else if (c == '=' && peekc() == '=' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_EQL_EQUALS_T);
    else if (c == '&' && peekc() == '&' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BOOL_AND_T);
    else if (c == '|' && peekc() == '|' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BOOL_OR_T);
    else if (c == '>' && peekc() == '>' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BIT_SHIFT_RIGHT_T);
    else if (c == '<' && peekc() == '<' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BIT_SHIFT_RIGHT_T);
    else if (c == '!' && peekc() == '=' && nextc() != '\0') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_EQL_NOT_EQUALS_T);

    else if (c == '+') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_ART_ADD_T);
    else if (c == '-') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_ART_SUB_T);
    else if (c == '*') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_ART_MUL_T);
    else if (c == '/') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_ART_DIV_T);
    else if (c == '%') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_ART_MOD_T);
    else if (c == '&') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BIT_AND_T);
    else if (c == '|') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BIT_OR_T);
    else if (c == '^') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BIT_XOR_T);
    else if (c == '~') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BIT_NOT_T);
    else if (c == '>') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BOOL_MORE_T);
    else if (c == '<') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BOOL_LESS_T);
    else if (c == '!') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_BOOL_NOT_T);
    else if (c == '=') *token = mktoken(MTK_OPERATOR_T, (void*) MOPR_ASSIGN_T);
    else 
      return 0;
    return 1;

  /* group token */
  }else if (c == '(')
  {
    array* tokens = array_new(8);
    bool ended = false;
    while (hasc())
    {
      c = peekc();

      /* check if ended */
      if (c == ')' && nextc() != '\0')
      {
	ended = true;
	break;
      }

      mtoken* t = NULL;
      if (MILEX_nexttoken(data, index, lpos, cpos, len, file, script, last_token, &t) != 1)
	return 0;
      if (t == NULL)
	continue;

      last_token = t;
      array_push(tokens, t);
    }

    /* check if ended */
    if (!ended)
    {
      mkferr("'(' not ended. expected ')'.");
      return 0;
    }

    *token = mktoken(MTK_GROUP_T, tokens);
    return 1;

  /* scope token */
  }else if (c == '{')
  {
    array* tokens = array_new(8);
    bool ended = false;
    while (hasc())
    {
      c = peekc();

      /* check if ended */
      if (c == '}' && nextc() != '\0')
      {
	ended = true;
	break;
      }
      
      mtoken* t = NULL;
      if (MILEX_nexttoken(data, index, lpos, cpos, len, file, script, last_token, &t) != 1)
	return 0;
      if (t == NULL)
	continue;

      last_token = t;
      array_push(tokens, t);
    }

    /* check if ended */
    if (!ended)
    {
      mkferr("'{' not ended. expected '}'");
      return 0;
    }

    *token = mktoken(MTK_SCOPE_T, tokens);
    return 1;
  }else if (c == '[')
  {

  /* comma token */
  }else if (c == ',')
  {
    *token = mktoken(MTK_COMMA_T, NULL);
    return 1;

  /* dot token */
  }else if (c == '.')
  {
    *token = mktoken(MTK_DOT_T, NULL);
    return 1;

  /* end token */
  }else if (c == ';')
  {
    *token = mktoken(MTK_END_T, NULL);
    return 1;
  }

  /* comment */
  else if (c == '/')
  {
    c = nextc();
    bool ended = false;
    if (c == '/')
    {
      while (hasc())
      { 
	if (nextc() == '\n')
	{
	  ended = true;
	  break;
	}
      }
    }else if (c == '*')
    {
      while (hasc())
      {
	c = nextc();
	if (c == '*' && nextc() == '/')
	{
	  ended = true;
	  break;
	}
      }
    }else
    {
      mkferr("expected comment start-point. here:");
      return 0;
    }

    /* check if comment ended */
    if (!ended)
      mkfwarn(":: comment not ended.");
    return 1;
  }else if (is_blank(c))
    return 1;
  mkferr("unknown character '%c'.");
  return 0;
}
