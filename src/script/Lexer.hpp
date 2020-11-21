#ifndef MI_LEXER_HPP
  #define MI_LEXER_HPP

#include "Token.hpp"
#include "Storage.hpp"

#include <lme/iterator.hpp>

#include <string>
#include <vector>

namespace makeit {

  class Lexer {

    enum Flag : uint8_t {
      NO_BREAK = 1
    };

  public:

    static int make(me::File* file, std::string &source, std::vector<Token*> &tokens, Storage* storage);

  private:

    static int next_token(me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags);

  };


}

#endif
