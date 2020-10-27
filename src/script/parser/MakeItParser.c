#include "MakeItParser.h"

#include <me/mestr.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int MI_parse_tokens(struct mi_parser* parser, struct array* tokens)
{
  struct iterator* iter = meiter_new(tokens->entries, tokens->size);

  while (meiter_has(iter))
  {
    struct mi_token* token = meiter_next(iter);
    if (MI_parse_token(parser, token) != 1)
      return 0;
  }
  return 1;
}

// TODO: clean up function args
int MI_parse_token(struct mi_parser* parser, struct mi_token* token)
{
  /* literial token */
  if (token->type == MI_TOK_LITERIAL)
  {
    struct array* v = memap_pull(parser->script->variables, token->value);

    if ((parser->status & MI_STATUS_APPEND) > 0 || (parser->status & MI_STATUS_FUNCTION) > 0 || (parser->status & MI_STATUS_VARIABLE) > 0)
    {
      /* check if variable exists */
      if (v == NULL)
      {
	printf(":: undefined variable '%s'.\n", token->value);
	return 0;
      }

      if ((parser->status & MI_STATUS_APPEND) > 0)
	mearr_push_all(parser->v.arr, v); // append the content of variable to current variable
      if ((parser->status & MI_STATUS_FUNCTION) > 0)
	parser->f.arr->entries[parser->f.arr->size - 1] = v; // set the current arg to variable
      return 1;
    }

    if (v == NULL)
      v = mearr_new(8);

    parser->v.name = token->value;
    parser->v.arr = v;

    memap_push(parser->script->variables, parser->v.name, parser->v.arr);

    parser->status |= MI_STATUS_VARIABLE;

  /* call token */
  }else if (token->type == MI_TOK_CALL)
  {
    parser->f.name = token->value;
    parser->f.arr = mearr_new(16);
    mearr_push(parser->f.arr, mearr_new(4));

    parser->status |= MI_STATUS_FUNCTION;

  /* operator token */
  }else if (token->type == MI_TOK_OPERATOR)
  {
    /* check if variable specified */
    if ((parser->status & MI_STATUS_VARIABLE) <= 0)
    {
      printf(":: 'variable' flag not active.\n");
      return 0;
    }

    enum mi_opr_type type = *((int*) token->value);

    /* clear the value if assign */
    if (type == MI_OPR_ASSIGN)
      mearr_clear(parser->v.arr); // TODO: delete the values

    parser->status |= MI_STATUS_APPEND;

  /* string token */
  }else if (token->type == MI_TOK_STRING)
  {
    if (MI_parser_string(parser, token->value, (char**) &token->value) != 1)
      return 0;

    /* check if status has 'append' */
    if ((parser->status & MI_STATUS_APPEND) > 0)
      mearr_push(parser->v.arr, token->value);

    /* check if status has 'function' */
    else if ((parser->status & MI_STATUS_FUNCTION) > 0)
      mearr_push(parser->f.arr->entries[parser->f.arr->size - 1], token->value);
    else
    {
      printf(":: 'append' or 'function' flag not active.\n");
      return 0;
    }

  /* comma token */
  }else if (token->type == MI_TOK_COMMA)
  {
    if ((parser->status & MI_STATUS_FUNCTION) <= 0)
    {
      printf(":: 'function' flag not active.\n");
      return 0;
    }

    mearr_push(parser->f.arr, mearr_new(4));

  /* end token */
  }else if (token->type == MI_TOK_END)
  {
    if ((parser->status & MI_STATUS_FUNCTION) > 0)
    {
      struct mi_function* f = memap_pull(parser->script->functions, parser->f.name);

      /* check if function was found */
      if (f == NULL)
      {
	printf(":: undefined function '%s'.\n", parser->f.name);
	return 0;
      }

      if (f->func(parser->script, parser->f.arr) != 1)
	return 0;
    }

    parser->status = 0;
  }
  return 1;
}

int MI_parser_string(struct mi_parser* parser, char* str, char** out)
{
  uint32_t len = strlen(str);

  /* if string is empty do nothing */
  if (len == 0)
  {
    *out = str;
    return 1;
  }

  struct string* str2 = mestr_new(len);
  struct string* ref_str = mestr_new(16);
  bool ref = false;

  for (uint32_t i = 0; i < len; i++)
  {
    if (str[i] == '$' && i < len - 1 && str[i + 1] == '(')
    {
      ref = true;
      i++;
    }else if (ref && str[i] == ')')
    {
      char* var_name = mestr_extrd(ref_str);
      struct array* var = memap_pull(parser->script->variables, var_name);

      /* check if variable was not found */
      if (var == NULL)
      {
	/* if var name is 'CURRENT_DIR' append the directory */
	if (strcmp("DIR", var_name) == 0)
	  mestr_apnd(str2, me_file_directory_path(parser->file));

	/* if var name is 'FILE' append the file name */
	else if (strcmp("FILE", var_name) == 0)
	  mestr_apnd(str2, parser->file->path);
	else
	{
	  printf(":: undefined variable '%s'.\n", var_name);
	  return 0;
	}

      /* if variable was found, append all stuff */
      }else
      {
	for (uint32_t i = 0; i < var->size; i++)
	{
	  mestr_apnd(str2, var->entries[i]);
	  if (i < var->size - 1)
	    mestr_apndc(str2, ' ');
	}
      }

      ref = false;
    }else if (ref)
      mestr_apndc(ref_str, str[i]);
    else
      mestr_apndc(str2, str[i]);
  }

  /* delete the old string */
  free(str);

  *out = mestr_extrd(str2);
  return 1;
}

struct mi_parser* MI_parser_new(struct file* file, struct mi_script* script)
{
  struct mi_parser* parser = malloc(sizeof(struct mi_parser));
  parser->file = file;
  parser->script = script;
  parser->status = 0;
  return parser;
}
