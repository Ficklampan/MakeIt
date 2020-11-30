#ifndef MAKEIT_TOKEN_HPP
  #define MAKEIT_TOKEN_HPP

#include "TokenLocation.hpp"
#include "Variable.hpp"
#include "Wildcard.hpp"

#include <lme/file.hpp>

namespace makeit {

  struct Token {

    /* Token Punctuator Type */
    enum Punctuator {
      L_PAREN, R_PAREN,

      EQUAL,
      EQUAL_EQUAL,
      PLUS_EQUAL,
      MINUS_EQUAL,
      STAR_EQUAL,
      SLASH_EQUAL,
      PERCENT_EQUAL,
      AND_AND,
      VBAR_VBAR,

      EXCLMARK
    };

    /* Token Type */
    enum Type {
      LITERIAL,
      CONSTANT,
      CALL,
      FOREACH,
      IF,
      THEN,
      ELSE,
      END,
      PUNCTUATOR,
      WILDCARD,
      BREAK
    } type;

    /* Token Value */
    union {
      void* v;
      std::string* s;
      int* i;
      Variable* c;
      Wildcard* w;
    } value;

    TokenLocation location;

    Token(Type type, void* value, TokenLocation location);

    static const char* type_name(Type type);

  };

}

#endif
