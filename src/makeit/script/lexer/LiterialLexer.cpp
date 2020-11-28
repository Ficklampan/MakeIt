#include "Lexer.hpp"

#include "../../Config.hpp"

extern makeit::Config config;

int makeit::Lexer::tokenize_literial(char c, Iterator &source, Token* &token, Storage* storage, uint8_t flags)
{
  MIDEBUG(2, "[Lexer] > next token is LITERIAL\n");

  uint32_t length = 1;
  LEXER_NEXT_STRING(if (!LEXER_IS_NAME(c)) { break; });

  std::string* literial = new std::string(&source.peek() - length, length);

  bool bt = literial->compare("true") == 0;
  bool bf = literial->compare("false") == 0;

  if (bt || bf)
  {
    LEXER_NEW_TOKEN(Token::CONSTANT, new Variable(&token->location, Variable::INTEGER, new int(bt)));
  }else if (c == ':')
  {
    source.next();
    LEXER_NEW_TOKEN(Token::CALL, literial);
  }else if (literial->compare("if") == 0)
  {
    LEXER_NEW_TOKEN(Token::IF, nullptr);
  }else if (literial->compare("then") == 0)
  {
    LEXER_NEW_TOKEN(Token::THEN, nullptr);
  }else if (literial->compare("else") == 0)
  {
    LEXER_NEW_TOKEN(Token::ELSE, nullptr);
  }else if (literial->compare("end") == 0)
  {
    LEXER_NEW_TOKEN(Token::END, nullptr);
  }else
    LEXER_NEW_TOKEN(Token::LITERIAL, literial);

  MIDEBUG(2, "[Lexer] > LITERIAL token created\n");
  return 1;
}
