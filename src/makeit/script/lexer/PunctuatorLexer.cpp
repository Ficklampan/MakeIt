#include "Lexer.hpp"

#include "../../Config.hpp"

extern makeit::Config config;

int makeit::Lexer::tokenize_punctuator(char c, me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags)
{
  MIDEBUG(2, "[Lexer] > next token is PUNCTUATOR\n");

  if (LEXER_IS_PUNCTUATOR(source.peek()))
  {
    char c2 = source.peek();

#define LEXER_NEW_PUNCTUATOR(t) ({ source.next(); token = new Token(Token::PUNCTUATOR, new int(t), TokenLocation(location)); return 1; })

    if (c == '=' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::EQUAL_EQUAL);
    else if (c == '+' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::PLUS_EQUAL);
    else if (c == '-' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::MINUS_EQUAL);
    else if (c == '*' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::STAR_EQUAL);
    else if (c == '/' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::SLASH_EQUAL);
    else if (c == '%' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::PERCENT_EQUAL);
    else if (c == '&' && c2 == '&') LEXER_NEW_PUNCTUATOR(Token::AND_AND);
    else if (c == '|' && c2 == '|') LEXER_NEW_PUNCTUATOR(Token::VBAR_VBAR);
  }

  if (c == '(') token = new Token(Token::PUNCTUATOR, new int(Token::L_PAREN), TokenLocation(location));
  else if (c == ')') token = new Token(Token::PUNCTUATOR, new int(Token::R_PAREN), TokenLocation(location));
  else if (c == '=') token = new Token(Token::PUNCTUATOR, new int(Token::EQUAL), TokenLocation(location));
  else if (c == '!') token = new Token(Token::PUNCTUATOR, new int(Token::EXCLMARK), TokenLocation(location));

  if (token == nullptr)
  {
    MIDEBUG(2, "[Lexer] > [PUNCTUATOR] > no match\n");
  }else
  {
    MIDEBUG(2, "[Lexer] > PUNCTUATOR token created\n");
  }

#undef LEXER_NEW_PUNCTUATOR
  return 1;
}
