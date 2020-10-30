#ifndef TOKEN_LEXER_HPP
  #define TOKEN_LEXER_HPP

#include "Token.hpp"

#include <me/iterator.hpp>

namespace mi {

  class TokenLexer {

  private:

    me::Iterator<char>* source;

  public:

    TokenLexer(me::Iterator<char>* source);

    /* maybe not the right word but it sounds cool ;) */
    int validate(Token* token);

    int validate_identifier(Token* token);
    int validate_string(Token* token);
    int validate_number(Token* token);
    int validate_punctuator(Token* token);

  };

}

#endif
