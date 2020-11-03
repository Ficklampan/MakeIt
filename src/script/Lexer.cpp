#include "Lexer.hpp"

#define LEXER_IS_NAME(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
#define LEXER_IS_DIGIT(c) (c >= '0' && c <= '9')
#define LEXER_IS_PUNCTUATOR(c) (c == '=' || c == '+' || c == ':')
#define LEXER_IS_EMPTY(c) (c == ' ' || c == '\t' || c == '\n')

int MI::Lexer::make(std::string &source, std::vector<Token*> &tokens)
{
  location = { &source, 0, 0, 0, 0, };

  me::Iterator<char> source_iter((char*) source.data(), source.size(), [](void* ptr, char &c, uint32_t &i) {
      MI::Token::Location* location = (MI::Token::Location*) ptr;
      if (c == '\n')
      {
	location->column = 0;
	location->line++;
      }else
	location->column++;
      location->pos++;
  }, &location);

  while (source_iter.hasNext())
  {
    Token* token = nullptr;
    if (!next_token(source_iter, token))
      return 0;

    if (token != nullptr)
      tokens.push_back(token);
  }
  return 1;
}

int MI::Lexer::next_token(me::Iterator<char> &source, Token* &token)
{
  location.first = location.pos;
  char c = source.next();

  /* Token::LITERIAL */
  if (LEXER_IS_NAME(c))
  {
    std::string* literial = new std::string;
    literial->push_back(c);

    while (source.hasNext())
    {
      c = source.peek();

      if (!LEXER_IS_NAME(c))
        break;

      literial->push_back(source.next());
    }

    token = new Token(Token::LITERIAL, literial, Token::Location(location));
    return 1;

  /* Token::CONSTANT STRING */
  }else if (c == '"')
  {
    std::string* str = new std::string;

    while (source.hasNext())
    {
      c = source.next();

      if (c == '"')
        break;

      str->push_back(c);
    }

    token = new Token(Token::CONSTANT, new Constant(Constant::STRING, str), Token::Location(location));
    return 1;

  /* Token::CONSTANT INTEGER */
  }else if (LEXER_IS_DIGIT(c))
  {
    // TODO

    return 1;

  /* Token::CONSTANT STRING_LIST */
  }else if (c == '[')
  {
    std::vector<Constant*>* elements = new std::vector<Constant*>;

    while (source.hasNext())
    {
      c = source.peek();

      if (c == ']')
      {
	source.next();
	break;
      }

      Token* token = nullptr;
      if (!next_token(source, token))
	return 0;

      if (token == nullptr)
	continue;

      /* [Error] expected constant */
      if (token->type != Token::CONSTANT)
      {
	printf(":: expected constant.\n");
	return 0;
      }

      elements->push_back(token->value.c);
    }

    token = new Token(Token::CONSTANT, new Constant(Constant::LIST, elements), Token::Location(location));
    return 1;

  /* Token::PUNCTUATOR */
  }else if (LEXER_IS_PUNCTUATOR(c))
  {
    if (LEXER_IS_PUNCTUATOR(source.peek()))
    {
      char c2 = source.peek();
#ifndef LEXER_NEW_PUNCTUATOR
  #define LEXER_NEW_PUNCTUATOR(t) ({ source.next(); token = new Token(Token::PUNCTUATOR, new int(t), Token::Location(location)); return 1; })
#endif
      if (c == '+' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::PLUS_EQUAL);
      else if (c == '-' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::MINUS_EQUAL);
      else if (c == '*' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::STAR_EQUAL);
      else if (c == '/' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::SLASH_EQUAL);
      else if (c == '%' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::PERCENT_EQUAL);
    }

    if (c == '(') token = new Token(Token::PUNCTUATOR, new int(Token::L_PAREN), Token::Location(location));
    else if (c == ')') token = new Token(Token::PUNCTUATOR, new int(Token::R_PAREN), Token::Location(location));
    else if (c == '=') token = new Token(Token::PUNCTUATOR, new int(Token::EQUAL), Token::Location(location));

    return 1;

  /* Token::BREAK */
  }else if (c == '\n' || c == ';')
  {
    token = new Token(Token::BREAK, nullptr, Token::Location(location));
    return 1;

  /* New Line / Comment */
  }else if (c == '\\' || c == '#')
  {
    bool comment = c == '#';

    while (source.hasNext())
    {
      c = source.next();
      
      if (c == '\n')
      {
        break;
      }
    }

    if (comment)
      token = new Token(Token::BREAK, nullptr, Token::Location(location));
    return 1;

  /* Error */
  }else if (!LEXER_IS_EMPTY(c))
  {
    printf(":: unknown symbol '%c'.\n", c);
    return 0;
  }
  return 1;
}
