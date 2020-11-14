#ifndef MI_TOKEN_HPP
  #define MI_TOKEN_HPP

#include "Variable.hpp"

#include <lme/file.hpp>

namespace MI {

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
      PUNCTUATOR,
      BREAK
    } type;

    /* Token Value */
    union {
      void* v;
      std::string* s;
      int* i;
      Constant* c;
    } value;

    /* Token Location in file */
    struct Location {
      me::File* file;
      std::string* source;
      uint32_t pos, line, column, first;
    } location;

    Token(Type type, void* value, Location location);

  };

}

#endif
