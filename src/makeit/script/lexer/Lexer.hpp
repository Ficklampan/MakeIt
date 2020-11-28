#ifndef MAKEIT_LEXER_HPP
  #define MAKEIT_LEXER_HPP

#include "../Token.hpp"
#include "../Storage.hpp"

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

#define LEXER_NEW_TOKEN(t, v) { \
  token = new Token(t, v, TokenLocation(source)); \
  token->location.size = source.index; \
  token->location.line = source.start_line; \
  token->location.column = source.start_column; \
  if (t == Token::CONSTANT) \
  { \
    token->value.c->location = &token->location; \
  } \
}

namespace makeit {

  class Lexer {

  public:

    struct Iterator : TokenLocation {

      uint32_t start_line = 1, start_column = 0;

      size_t index = 0;

      Iterator(me::File* file, std::string* source)
	: TokenLocation(file, source)
      {
      }

      const bool hasNext() const;
      char& peek() const;
      char& next();

    };

  public:

    enum Flag : uint8_t {
      NO_BREAK = 1,
      NO_ERROR = 1 << 1,
      NO_WARNING = 1 << 2
    };

    static int tokenize_tokens(me::File* file, std::string &source, std::vector<Token*> &tokens, Storage* storage, uint8_t flags);
    static int tokenize_token(Iterator &source, Token* &token, Storage* storage, uint8_t flags);

  private:

    static int tokenize_literial(char c, Iterator &source, Token* &token, Storage* storage, uint8_t flags);
    static int tokenize_constant(char c, Iterator &source, Token* &token, Storage* storage, uint8_t flags);
    static int tokenize_punctuator(char c, Iterator &source, Token* &token, Storage* storage, uint8_t flags);
    static int tokenize_wildcard(char c, Iterator &source, Token* &token, Storage* storage, uint8_t flags);

  };


}

#endif
