#include "Lexer.hpp"

#include "../../Config.hpp"

extern makeit::Config config;

int makeit::Lexer::tokenize_literial(char c, me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags)
{
  MIDEBUG(2, "[Lexer] > next token is LITERIAL\n");

  uint32_t length = 1;
  LEXER_NEXT_STRING(if (!LEXER_IS_NAME(c)) { break; });

  std::string* literial = new std::string(&source.peek() - length, length);

  bool bt = literial->compare("true") == 0;
  bool bf = literial->compare("false") == 0;

  if (bt || bf)
  {
    token = new Token(Token::CONSTANT, new Variable(Variable::INTEGER, new int(bt)), TokenLocation(location));
  }else if (c == ':')
  {
    source.next();
    token = new Token(Token::CALL, literial, TokenLocation(location));
  }else if (literial->compare("if") == 0)
  {
    token = new Token(Token::IF, nullptr, TokenLocation(location));
  }else if (literial->compare("then") == 0)
  {
    token = new Token(Token::THEN, nullptr, TokenLocation(location));
  }else if (literial->compare("else") == 0)
  {
    token = new Token(Token::ELSE, nullptr, TokenLocation(location));
  }else if (literial->compare("end") == 0)
  {
    token = new Token(Token::END, nullptr, TokenLocation(location));
  }else
    token = new Token(Token::LITERIAL, literial, TokenLocation(location));

  MIDEBUG(2, "[Lexer] > LITERIAL token created\n");
  return 1;
}
