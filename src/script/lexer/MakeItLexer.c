#include "MakeItLexer.h"

#include "../MakeItScript.h"

#include <me/mestr.h>

#include <stdlib.h>
#include <stdio.h>

int MI_make_tokens(struct mi_lexer* lexer, char* data, uint32_t length)
{
  struct mi_token_loc location = { .index = 0, .line = 0, .column = 0, .length = 0 };
  while (location.index < length && data[location.index] != '\0')
  {
    struct mi_token* token = NULL;

    if (MI_next_token(data, length, &location, &token) != 1)
      return 0;

    if (token != NULL)
      mearr_push(lexer->tokens, token);
  }
  return 1;
}

static char MI_escape_char(char c)
{
  switch (c)
  {
    case '0': return '\0';
    case 'n': return '\n';
    case 't': return '\t';
    case '"': return '"';
    case '\'': return '\'';
    case '\\': return '\\';
  }
  return c;
}

static char MI_next_char(char* data, struct mi_token_loc* location)
{
  char c = data[location->index];

  //printf("c[%c]\n", c);

  location->index+=1;
  if (c == '\n')
  {
    location->line++;
    location->column = 0;
  }
  location->column+=1;
  return c;
}

int MI_next_token(char* data, uint32_t length, struct mi_token_loc* location, struct mi_token** token)
{
  uint32_t start_index = location->index;
  uint32_t start_line = location->line;
  uint32_t start_column = location->column;

#define NEXTC() (MI_next_char(data, location))
#define PEEKC() (data[location->index])
#define HASC() (location->index < length)
#define ISNAME(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')
#define ISDIGIT(c, d) ((c >= '0' && c <= '9') || (d && c == '.'))
#define ISOPR(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || \
		  c == '&' || c == '|' || c == '<' || c == '>' || c == '^' || c == '~')
#define ISBLANK(c) (c == ' ' || c == '\t')

#define IVALUE(i) ({int* v = (int*) calloc(sizeof(int), 1); *v = i; v; })
#define MKTOKEN(t, v) (MI_token_new(t, v, MI_token_loc_new(start_index, start_line, start_column, location->index - start_index)))

  char c = NEXTC();

  /* literial/call token */
  if (ISNAME(c))
  {
    struct string* literial = mestr_new(32);
    mestr_apndc(literial, c);
    
    while (HASC())
    {
      c = PEEKC();

      if (!ISNAME(c) && !ISDIGIT(c, false))
	break;

      mestr_apndc(literial, NEXTC());
    }


    bool call = c == ':' && NEXTC() == ':';

    *token = MKTOKEN(call ? MI_TOK_CALL : MI_TOK_LITERIAL, mestr_extrd(literial));
    return 1;

  /* string token */
  }else if (c == '"')
  {
    struct string* string = mestr_new(128);

    while (HASC())
    {
      c = NEXTC();

      if (c == '"')
	break;

      if (c == '\\')
	c = MI_escape_char(NEXTC());

      mestr_apndc(string, c);
    }

    *token = MKTOKEN(MI_TOK_STRING, mestr_extrd(string));
    return 1;

  /* number token */
  }else if (ISDIGIT(c, false))
  {
    // TODO:
    return 1;

  /* operator token */
  }else if (ISOPR(c))
  {
    if (ISOPR(PEEKC()))
    {
      char c2 = NEXTC();

      if (c == '+' && c2 == '=') *token = MKTOKEN(MI_TOK_OPERATOR, IVALUE(MI_OPR_APPEND));

    }else
    {
      if (c == '=') *token = MKTOKEN(MI_TOK_OPERATOR, IVALUE(MI_OPR_ASSIGN));
    }
    return 1;

  /* comma token */
  }else if (c == ',')
  {
    *token = MKTOKEN(MI_TOK_COMMA, NULL);
    return 1;

  /* end token */
  }else if (c == '\n')
  {
    *token = MKTOKEN(MI_TOK_END, NULL);
    return 1;

  /* next line */
  }else if (c == '\\')
  {
    while (HASC())
    {
      c = NEXTC();

      if (c == '\n')
	break;
    }
    return 1;
  }else if (ISBLANK(c))
    return 1;

  printf(":: unknown character '%c'.\n", c);
  return 0;
}

struct mi_lexer* MI_lexer_new(struct array* tokens)
{
  struct mi_lexer* lexer = calloc(sizeof(struct mi_lexer), 1);
  lexer->tokens = tokens;
  return lexer;
}

struct mi_token* MI_token_new(enum mi_token_type type, void* value, struct mi_token_loc* location)
{
  struct mi_token* token = calloc(sizeof(struct mi_token), 1);
  token->type = type;
  token->value = value;
  token->location = location;
  return token;
}

struct mi_token_loc* MI_token_loc_new(uint32_t index, uint32_t line, uint32_t column, uint32_t length)
{
  struct mi_token_loc* location = calloc(sizeof(struct mi_token_loc), 1);
  location->index = index;
  location->line = line;
  location->column = column;
  location->length = length;
  return location;
}

char* MI_token_info(enum mi_token_type type)
{
  switch (type)
  {
    case MI_TOK_CALL:
      return "CALL";
    case MI_TOK_LITERIAL:
      return "LITERIAL";
    case MI_TOK_OPERATOR:
      return "OPERATOR";
    case MI_TOK_STRING:
      return "STRING";
    case MI_TOK_COMMA:
      return "COMMA";
    case MI_TOK_END:
      return "END";
  }
  return "?";
}
