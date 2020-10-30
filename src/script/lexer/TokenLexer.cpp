#include "TokenLexer.hpp"

#include "Lexer.def"

#include "../Script.hpp"

#include <string>
#include <cstring>

mi::TokenLexer::TokenLexer(me::Iterator<char>* source)
{
  this->source = source;
}

int mi::TokenLexer::validate(Token* token)
{
  switch (token->type)
  {
    case Token::IDENTIFIER:
      return validate_identifier(token);
    case Token::STRING:
      return validate_string(token);
    case Token::INTEGER:
      return validate_number(token);
    case Token::PUNCTUATOR:
      return validate_punctuator(token);
    default:
      source->next();
      return 1;
  }
}

int mi::TokenLexer::validate_identifier(Token* token)
{
  std::string* identifier = new std::string();

  while (source->hasNext())
  {
    char c = source->peek();

    if (!LEXER_IS_NAME(c))
      break;

    identifier->push_back(source->next());
  }

  token->value.v = identifier;

  return 1;
}

int mi::TokenLexer::validate_string(Token* token)
{
  source->next(); // skipping the first "

  std::string str;

  while (source->hasNext())
  {
    char c = source->peek();

    if (c == '"')
      break;

    str.push_back(source->next());
  }

  token->value.v = new char[str.size() + 1];
  std::strcpy((char*) token->value.v, str.c_str());

  source->next(); // skipping the last "

  return 1;

}

int mi::TokenLexer::validate_number(Token* token)
{
  std::string* num_str = new std::string();

  while (source->hasNext())
  {
    char c = source->peek();

    if (!LEXER_IS_DIGIT(c))
      break;

    num_str->push_back(source->next());
  }

  token->value.i = new int(atoi(num_str->c_str()));

  return 1;
}

int mi::TokenLexer::validate_punctuator(Token* token)
{
  char c = source->next();

  switch (c)
  {
    /* EQUALS */
    case '=':
      token->value.i = new int(EQUAL);
      break;

    /* PLUS */
    case '+':
      if (LEXER_IS_PUNCTUATOR(source->peek()))
      {
	c = source->next();
	if (c == '=') token->value.i = new int(PLUS_EQUAL);
      }else
	token->value.i = new int(PLUS);
      break;

    /* MINUS */
    case '-':
      if (LEXER_IS_PUNCTUATOR(source->peek()))
      {
	c = source->next();
	if (c == '=') token->value.i = new int(MINUS_EQUAL);
      }else
	token->value.i = new int(MINUS);

      break;

    /* COMMA */
    case ',':
      token->value.i = new int(COMMA);
      break;

    /* LEFT SQUARE */
    case '[':
      token->value.i = new int(L_SQUARE);
      break;
    
    /* RIGHT SQUARE */
    case ']':
      token->value.i = new int(R_SQUARE);
      break;

    /* LEFT BRACE */
    case '{':
      token->value.i = new int(L_BRACE);
      break;
    
    /* RIGHT BRACE */
    case '}':
      token->value.i = new int(R_BRACE);
      break;

    /* LEFT PAREN */
    case '(':
      token->value.i = new int(L_PAREN);
      break;
    
    /* RIGHT PAREN */
    case ')':
      token->value.i = new int(R_PAREN);
      break;
  }
  return 1;
}
