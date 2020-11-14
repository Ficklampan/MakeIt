#ifndef MI_LEXER_HPP
  #define MI_LEXER_HPP

#include "Token.hpp"

#include <lme/iterator.hpp>

#include <string>
#include <vector>

namespace MI {

  class Lexer {

  public:

    static int make(me::File* file, std::string &source, std::vector<Token*> &tokens);

  private:

    static int next_token(me::Iterator<char> &source, Token::Location &location, Token* &token);

  };


}

#endif
