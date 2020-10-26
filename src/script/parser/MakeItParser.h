#ifndef MI_PARSER_H
  #define MI_PARSER_H

#include "../lexer/MakeItLexer.h"
#include "../MakeItScript.h"

#include <me/meiter.h>
#include <me/mearr.h>
#include <me/mefil.h>

enum mi_status {
  MI_STATUS_VARIABLE = 1,
  MI_STATUS_FUNCTION = 1 << 1,
  MI_STATUS_APPEND = 1 << 2
};

struct mi_parser {
  struct mi_script* script;
  struct file* file;
  uint8_t status;
  struct {
    char* name;
    struct array* arr;
  } v, f;
};

int MI_parse_tokens(struct mi_parser* parser, struct array* tokens);
int MI_parse_token(struct mi_parser* parser, struct mi_token* token);
int MI_parser_string(struct mi_parser* parser, char* str, char** out);

struct mi_parser* MI_parser_new(struct file* file, struct mi_script* script);

#endif
