#include "Lexer.hpp"
#include "TokenLexer.hpp"

#include "Lexer.def"

mi::Token::Token(Type type, void* value)
{
  this->type = type;
  this->value.v = value;
}

mi::Lexer::Lexer(const std::string &source)
{
  this->source = new me::Iterator<char>((char*) source.c_str(), source.size());
  this->tokenLexer = new TokenLexer(this->source);
}

std::vector<mi::Token*>& mi::Lexer::getTokens()
{
  return tokens;
}

int mi::Lexer::makeTokens()
{
  while (source->hasNext())
  {
    Token token(Token::UNKNOWN, nullptr);

    if (!nextToken(&token))
      return 0;

    if (token.type != Token::UNKNOWN)
      tokens.push_back(new Token(token));
  }
  return 1;
}

int mi::Lexer::nextToken(Token* token)
{
  char c = source->peek();

  /* IDENTIFIER */
  if (LEXER_IS_NAME(c))
    token->type = Token::IDENTIFIER;

  /* NUMBER */
  else if (LEXER_IS_DIGIT(c))
    token->type = Token::INTEGER;

  /* STRING */
  else if (c == '"')
    token->type = Token::STRING;

  /* PUNCTUATOR */
  else if (LEXER_IS_PUNCTUATOR(c))
    token->type = Token::PUNCTUATOR;

  /* RESET */
  else if (c == ';')
    token->type = Token::RESET;

  /* NEW LINE */
  else if (c == '\\')
  {
    while (source->hasNext())
    {
      c = source->next();
      if (c == '\n')
	break;
    }

  /* COMMENT and RESET */
  }else if (c == '#')
  {
    token->type = Token::RESET;
    while (source->hasNext())
    {
      c = source->next();
      if (c == '\n')
	break;
    }
  }else if (LEXER_IS_BLANK(c))
  {
    source->next();
    return 1;
  }else
  {
    printf(":: unknown character '%c'.\n", c);
    return 0;
  }

  if (!tokenLexer->validate(token))
    return 0;
  return 1;
}
