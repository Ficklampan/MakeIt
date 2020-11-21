#ifndef MAKEIT_LEXER_HPP
  #define MAKEIT_LEXER_HPP

#include "../Token.hpp"
#include "../Storage.hpp"

#include <lme/iterator.hpp>

#include <string>
#include <vector>

#define LEXER_IS_NAME(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
#define LEXER_IS_DIGIT(c) (c >= '0' && c <= '9')
#define LEXER_IS_PUNCTUATOR(c) (c == '=' || c == '+' || c == '&' || c == '|' || c == '!')
#define LEXER_IS_WILDCARD(c) (c == '?')
#define LEXER_IS_EMPTY(c) (c == ' ' || c == '\t' || c == '\n')

#define LEXER_NEXT_STRING(b) { \
  while (source.hasNext()) \
  { \
    c = source.peek(); \
    b \
    length++; \
    source.next(); \
  } \
}

namespace makeit {

  class Lexer {

  public:

    enum Flag : uint8_t {
      NO_BREAK = 1,
      NO_ERROR = 1 << 1,
      NO_WARNING = 1 << 2
    };

    static int tokenize_tokens(me::File* file, std::string &source, std::vector<Token*> &tokens, Storage* storage, uint8_t flags);
    static int tokenize_token(me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags);

  private:

    static int tokenize_literial(char c, me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags);
    static int tokenize_constant(char c, me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags);
    static int tokenize_punctuator(char c, me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags);
    static int tokenize_wildcard(char c, me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags);

  };


}

#endif
