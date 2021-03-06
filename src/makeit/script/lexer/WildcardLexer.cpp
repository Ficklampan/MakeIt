#include "Lexer.hpp"

#include "../../Config.hpp"

extern makeit::Config config;

int makeit::Lexer::tokenize_wildcard(char c, Iterator &source, Token* &token, Storage* storage, uint8_t flags)
{
  if (c == '?')
  {
    MIDEBUG(2, "[Lexer] > next token is WILDCARD(DEFINED)\n");

    uint32_t length = 0;
    LEXER_NEXT_STRING(if (!LEXER_IS_NAME(c)) { break; });

    LEXER_NEW_TOKEN(Token::WILDCARD, new Wildcard(Wildcard::DEFINED, std::string(&source.peek() - length, length)));

    MIDEBUG(2, "[Lexer] > WILDCARD(DEFINED) token created\n");
    return 1;
  }
  return 1;
}
