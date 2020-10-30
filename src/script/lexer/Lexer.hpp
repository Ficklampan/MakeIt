#ifndef MI_LEXER_HPP
  #define MI_LEXER_HPP

#include "Token.hpp"
#include "TokenLexer.hpp"

#include <vector>
#include <string>

namespace mi {

  class Lexer {

  private:

    std::vector<Token*> tokens;
    me::Iterator<char>* source;
    TokenLexer* tokenLexer;

  public:

    Lexer(const std::string &source);

    std::vector<Token*>& getTokens();

    int makeTokens();
    int nextToken(Token* token);

  };

}

#endif
