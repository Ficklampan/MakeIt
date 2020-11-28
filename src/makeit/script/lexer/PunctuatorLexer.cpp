#include "Lexer.hpp"

#include "../../Config.hpp"

extern makeit::Config config;

int makeit::Lexer::tokenize_punctuator(char c, Iterator &source, Token* &token, Storage* storage, uint8_t flags)
{
  MIDEBUG(2, "[Lexer] > next token is PUNCTUATOR\n");

  if (LEXER_IS_PUNCTUATOR(source.peek()))
  {
    char c2 = source.peek();

#define LEXER_NEW_PUNCTUATOR(t) ({ source.next(); LEXER_NEW_TOKEN(Token::PUNCTUATOR, new int(t)); return 1; })

    if (c == '=' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::EQUAL_EQUAL);
    else if (c == '+' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::PLUS_EQUAL);
    else if (c == '-' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::MINUS_EQUAL);
    else if (c == '*' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::STAR_EQUAL);
    else if (c == '/' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::SLASH_EQUAL);
    else if (c == '%' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::PERCENT_EQUAL);
    else if (c == '&' && c2 == '&') LEXER_NEW_PUNCTUATOR(Token::AND_AND);
    else if (c == '|' && c2 == '|') LEXER_NEW_PUNCTUATOR(Token::VBAR_VBAR);
  }

  if (c == '(') LEXER_NEW_TOKEN(Token::PUNCTUATOR, new int(Token::L_PAREN))
  else if (c == ')') LEXER_NEW_TOKEN(Token::PUNCTUATOR, new int(Token::R_PAREN))
  else if (c == '=') LEXER_NEW_TOKEN(Token::PUNCTUATOR, new int(Token::EQUAL))
  else if (c == '!') LEXER_NEW_TOKEN(Token::PUNCTUATOR, new int(Token::EXCLMARK))

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
