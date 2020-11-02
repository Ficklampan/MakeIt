#ifndef MI_TOKEN_HPP
  #define MI_TOKEN_HPP

#include "Variable.hpp"

namespace MI {

  struct Token {

    enum Punctuator {
      L_PAREN, R_PAREN,

      EQUAL,
      PLUS_EQUAL,
      MINUS_EQUAL,
      STAR_EQUAL,
      SLASH_EQUAL,
      PERCENT_EQUAL
    };

    enum Type {
      LITERIAL,
      CONSTANT,
      PUNCTUATOR,
      BREAK
    } type;
    union {
      void* v;
      std::string* s;
      int* i;
      Constant* c;
    } value;

    Token(Type type, void* value)
    {
      this->type = type;
      this->value.v = value;
    }

    ~Token()
    {
    }

  };

}

#endif
