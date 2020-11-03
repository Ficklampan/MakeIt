#ifndef MI_LEXER_HPP
  #define MI_LEXER_HPP

#include "Token.hpp"

#include <lme/iterator.hpp>

#include <string>
#include <vector>

namespace MI {

  class Lexer {

  private:

    Token::Location location;

  public:

    int make(std::string &source, std::vector<Token*> &tokens);
    int next_token(me::Iterator<char> &source, Token* &token);

  };


}

#endif
