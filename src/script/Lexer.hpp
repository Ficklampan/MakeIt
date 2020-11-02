#ifndef MI_LEXER_HPP
  #define MI_LEXER_HPP

#include "Token.hpp"

#include <iterator.hpp>

#include <string>
#include <vector>

namespace MI {

  class Lexer {

  public:

    int make(me::Iterator<char> &source, std::vector<Token*> &tokens);
    int next_token(me::Iterator<char> &source, Token* &token);

  };


}

#endif
