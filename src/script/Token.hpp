#ifndef MI_TOKEN_HPP
  #define MI_TOKEN_HPP

#include "Variable.hpp"

#include <lme/file.hpp>

namespace makeit {

    /* Token Location in file */
  struct TokenLocation {
    me::File* file;
    std::string* source;
    uint32_t pos, line, column, first;
  };

  struct Token {

    /* Token Punctuator Type */
    enum Punctuator {
      L_PAREN, R_PAREN,

      EQUAL,
      PLUS_EQUAL,
      MINUS_EQUAL,
      STAR_EQUAL,
      SLASH_EQUAL,
      PERCENT_EQUAL
    };

    /* Token Type */
    enum Type {
      LITERIAL,
      CONSTANT,
      CALL,
      PUNCTUATOR,
      BREAK
    } type;

    /* Token Value */
    union {
      void* v;
      std::string* s;
      int* i;
      Variable* c;
    } value;

    TokenLocation location;

    Token(Type type, void* value, TokenLocation location);

    static const char* type_name(Type type);

  };

}

#endif
