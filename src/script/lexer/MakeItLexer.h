#ifndef MI_LEXER_H
  #define MI_LEXER_H

#include <me/mearr.h>

struct mi_lexer {
  struct array* tokens;
};

enum mi_token_type {
  MI_TOK_LITERIAL,
  MI_TOK_CALL,
  MI_TOK_OPERATOR,
  MI_TOK_STRING,
  MI_TOK_COMMA,
  MI_TOK_END
};

enum mi_opr_type {
  MI_OPR_ASSIGN,
  MI_OPR_APPEND
};

struct mi_token_loc {
  uint32_t index, line, column;
  uint32_t length;
};

struct mi_token {

  enum mi_token_type type;
  void* value;

  struct mi_token_loc* location;

};

int MI_make_tokens(struct mi_lexer* lexer, char* data, uint32_t length);
int MI_next_token(char* data, uint32_t length, struct mi_token_loc* location, struct mi_token** token);

struct mi_lexer* MI_lexer_new(struct array* tokens);
struct mi_token* MI_token_new(enum mi_token_type type, void* value, struct mi_token_loc* location);
struct mi_token_loc* MI_token_loc_new(uint32_t index, uint32_t line, uint32_t column, uint32_t length);

char* MI_token_info(enum mi_token_type type);

#endif
