#include "Lexer.hpp"

#include "Common.hpp"

#include "../Config.hpp"

#include <lme/list.hpp>

#define LEXER_IS_NAME(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
#define LEXER_IS_DIGIT(c) (c >= '0' && c <= '9')
#define LEXER_IS_PUNCTUATOR(c) (c == '=' || c == '+' || c == ':')
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

extern Config config;

int makeit::Lexer::make(me::File* file, std::string &source, std::vector<Token*> &tokens, Storage* storage)
{
  TokenLocation location = { file, &source, 0, 0, 0, 0, };

  me::Iterator<char> source_iter((char*) source.data(), source.size(), [](void* ptr, char &c, uint32_t &i) {
      TokenLocation* location = (TokenLocation*) ptr;
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
    if (!next_token(source_iter, location, token, storage, 0))
      return 0;

    if (token != nullptr)
      tokens.push_back(token);
  }
  return 1;
}

int makeit::Lexer::next_token(me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags)
{
  location.first = location.pos;
  char c = source.next();

  /* Token::LITERIAL / Token::CALL / Token::CONSTANT BOOLEAN */
  if (LEXER_IS_NAME(c))
  {
    MIDEBUG(2, "[Lexer] > next token is LITERIAL\n");

    uint32_t length = 1;
    LEXER_NEXT_STRING(if (!LEXER_IS_NAME(c)) { break; });

    std::string* literial = new std::string(&source.peek() - length, length);

    bool bt = literial->compare("true") == 0;
    bool bf = literial->compare("false") == 0;

    if (bt || bf)
      token = new Token(Token::CONSTANT, new Variable(Variable::INTEGER, new int(bt)), TokenLocation(location));
    else if (c == ':')
    {
      source.next();
      token = new Token(Token::CALL, literial, TokenLocation(location));
    }else
      token = new Token(Token::LITERIAL, literial, TokenLocation(location));

    MIDEBUG(2, "[Lexer] > LITERIAL token created\n");
    return 1;

  /* Token::CONSTANT STRING */
  }else if (c == '"')
  {
    MIDEBUG(2, "[Lexer] > next token is CONSTANT(STRING)\n");

    uint32_t length = 0;
    LEXER_NEXT_STRING(if (c == '"') { source.next(); break; });

    std::string* str = new std::string(&source.peek() - length - 1, length);

    token = new Token(Token::CONSTANT, new Variable(Variable::STRING, str), TokenLocation(location));

    MIDEBUG(2, "[Lexer] > CONSTANT(STRING) token created\n");
    return 1;

  /* Token::CONSTANT INTEGER */
  }else if (LEXER_IS_DIGIT(c))
  {
    // TODO

    return 1;

  /* Token::CONSTANT LIST */
  }else if (c == '[')
  {
    MIDEBUG(2, "[Lexer] > next token is CONSTANT(LIST)\n");

    std::vector<Variable*>* elements = new std::vector<Variable*>;

    /* append all next token constants to 'elements' */
    while (source.hasNext())
    {
      c = source.peek();

      if (c == ']')
      {
	source.next();
	break;
      }

      Token* token = nullptr;
      if (!next_token(source, location, token, storage, NO_BREAK))
	return 0;

      if (token == nullptr)
	continue;
      
      /* [Error] expected constant */
      if (token->type != Token::CONSTANT)
      {
	printError(token->location, "expected constant");
	return 0;
      }

      elements->push_back(token->value.c);
    }

    MIDEBUG(2, "[Lexer] > [CONSTANT(LIST)] > found %lu elements\n", elements->size());

    token = new Token(Token::CONSTANT, new Variable(Variable::LIST, elements), TokenLocation(location));

    MIDEBUG(2, "[Lexer] > CONSTANT(LIST) token created\n");
    return 1;

  /* Token::CONSTANT STRUCT */
  }else if (c == '{')
  {
    MIDEBUG(2, "[Lexer] > next token is CONSTANT(STRUCT)\n");

    std::map<std::string, Variable*>* st = new std::map<std::string, Variable*>;

    Token::Type expect = Token::LITERIAL;
    std::string identifier;

    while (source.hasNext())
    {
      c = source.peek();

      if (c == '}')
      {
	source.next();
	break;
      }

      Token* token = nullptr;
      if (!next_token(source, location, token, storage, NO_BREAK))
	return 0;

      if (token == nullptr)
	continue;

      if (token->type != expect)
      {
	printError(token->location, "expected type '%s' but found type '%s'", Token::type_name(expect), Token::type_name(token->type));
	return 0;
      }

      if (token->type == Token::LITERIAL)
      {
	identifier = *token->value.s;
	expect = Token::CONSTANT;
      }else if (token->type == Token::CONSTANT)
      {
	(*st)[identifier] = token->value.c;
	expect = Token::LITERIAL;
      }
    }

    MIDEBUG(2, "[Lexer] > [CONSTANT(STRUCT)] > found %lu elements\n", st->size());
   
    token = new Token(Token::CONSTANT, new Variable(Variable::STRUCT, st), TokenLocation(location));

    MIDEBUG(2, "[Lexer] > CONSTANT(STRUCT) token created\n");
    return 1;

  /* Token::CONSTANT REFERENCE */
  }else if (c == '$')
  {
    MIDEBUG(2, "[Lexer] > next token is CONSTANT(REFERENCE)\n");

    uint32_t length = 0;
    LEXER_NEXT_STRING(if (!LEXER_IS_NAME(c)) { break; });

    std::string* ref = new std::string(&source.peek() - length, length);

    token = new Token(Token::CONSTANT, new Variable(Variable::REFERENCE, ref), TokenLocation(location));

    MIDEBUG(2, "[Lexer] > CONSTANT(REFERENCE) token created\n");
    return 1;

  /* Token::PUNCTUATOR */
  }else if (LEXER_IS_PUNCTUATOR(c))
  {
    MIDEBUG(2, "[Lexer] > next token is PUNCTUATOR\n");

    if (LEXER_IS_PUNCTUATOR(source.peek()))
    {
      char c2 = source.peek();
#ifndef LEXER_NEW_PUNCTUATOR
  #define LEXER_NEW_PUNCTUATOR(t) ({ source.next(); token = new Token(Token::PUNCTUATOR, new int(t), TokenLocation(location)); return 1; })
#endif
      if (c == '+' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::PLUS_EQUAL);
      else if (c == '-' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::MINUS_EQUAL);
      else if (c == '*' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::STAR_EQUAL);
      else if (c == '/' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::SLASH_EQUAL);
      else if (c == '%' && c2 == '=') LEXER_NEW_PUNCTUATOR(Token::PERCENT_EQUAL);
    }

    if (c == '(') token = new Token(Token::PUNCTUATOR, new int(Token::L_PAREN), TokenLocation(location));
    else if (c == ')') token = new Token(Token::PUNCTUATOR, new int(Token::R_PAREN), TokenLocation(location));
    else if (c == '=') token = new Token(Token::PUNCTUATOR, new int(Token::EQUAL), TokenLocation(location));

    if (token == nullptr)
    {
      MIDEBUG(2, "[Lexer] > [PUNCTUATOR] > no match\n");
    }else
    {
      MIDEBUG(2, "[Lexer] > PUNCTUATOR token created\n");
    }

    return 1;

  /* Token::BREAK */
  }else if ((flags & NO_BREAK) == 0 && (c == '\n' || c == ';'))
  {
    MIDEBUG(2, "[Lexer] > next token is BREAK\n");

    token = new Token(Token::BREAK, nullptr, TokenLocation(location));

    MIDEBUG(2, "[Lexer] > BREAK token created\n");
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
      token = new Token(Token::BREAK, nullptr, TokenLocation(location));
    return 1;

  /* Error */
  }else if (!LEXER_IS_EMPTY(c))
  {
    printError(location, "unknown symbol '%c'", c);
    return 0;
  }
  return 1;
}
