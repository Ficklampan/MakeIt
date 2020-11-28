#include "Lexer.hpp"
#include "../../Text.hpp"
#include "../Script.hpp"

#include "../../Config.hpp"

extern makeit::Config config;

static char escape_char(char c)
{
  switch (c)
  {
    case 't': return '\t';
    case 'n': return '\n';
    case 'r': return '\r';
    case '0': return '\0';
    default: return c;
  }
}

int makeit::Lexer::tokenize_constant(char c, Iterator &source, Token* &token, Storage* storage, uint8_t flags)
{
  if (c == '"')
  {
    MIDEBUG(2, "[Lexer] > next token is CONSTANT(STRING)\n");

    std::string* str = new std::string;
    while (source.hasNext())
    {
      c = source.next();
      if (c == '"')
	break;
      if (c == '\\')
	c = escape_char(source.next());
      (*str) += c;
    }

    LEXER_NEW_TOKEN(Token::CONSTANT, new Variable(nullptr, Variable::STRING, str));

    MIDEBUG(2, "[Lexer] > CONSTANT(STRING) token created\n");
    return 1;

  /* Token::CONSTANT INTEGER */
  }else if (LEXER_IS_DIGIT(c) || c == '-')
  {
    MIDEBUG(2, "[Lexer] > next token is CONSTANT(INTEGER)\n");

    uint32_t length = 1;
    LEXER_NEXT_STRING(if (!LEXER_IS_DIGIT(c)) { break; });

    std::string number(&source.peek() - length, length);

    LEXER_NEW_TOKEN(Token::CONSTANT, new Variable(nullptr, Variable::INTEGER, new int(std::stoi(number))));

    MIDEBUG(2, "[Lexer] > CONSTANT(INTEGER) token created\n");
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
      tokenize_token(source, token, storage, NO_BREAK | flags);

      if (token == nullptr)
	continue;
      
      /* [Error] expected constant */
      if ((flags & NO_ERROR) == 0 && token->type != Token::CONSTANT)
      {
	throw Exception(&token->location, EEXPECTED_CONSTANT, { });
      }

      if (token->type == Token::CONSTANT)
	elements->push_back(token->value.c);
    }

    LEXER_NEW_TOKEN(Token::CONSTANT, new Variable(nullptr, Variable::LIST, elements));

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
      tokenize_token(source, token, storage, NO_BREAK | flags);

      if (token == nullptr)
	continue;

      if ((flags & NO_ERROR) == 0 && token->type != expect)
      {
	throw Exception(&token->location, EEXPECTED_TOKEN_TYPE, { Token::type_name(expect), Token::type_name(token->type) });
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

    LEXER_NEW_TOKEN(Token::CONSTANT, new Variable(nullptr, Variable::STRUCT, st));

    MIDEBUG(2, "[Lexer] > CONSTANT(STRUCT) token created\n");
    return 1;

  /* Token::CONSTANT REFERENCE */
  }else if (c == '$')
  {
    MIDEBUG(2, "[Lexer] > next token is CONSTANT(REFERENCE)\n");

    uint32_t length = 0;
    LEXER_NEXT_STRING(if (!LEXER_IS_NAME(c)) { break; });

    LEXER_NEW_TOKEN(Token::CONSTANT, new Variable(nullptr, Variable::REFERENCE, new std::string(&source.peek() - length, length)));

    MIDEBUG(2, "[Lexer] > CONSTANT(REFERENCE) token created\n");
    return 1;
  }
  return 1;
}
