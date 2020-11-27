#include "Lexer.hpp"
#include "../../Text.hpp"
#include "../Script.hpp"

#include "../../Config.hpp"

extern makeit::Config config;

int makeit::Lexer::tokenize_constant(char c, me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags)
{
  if (c == '"')
  {
    MIDEBUG(2, "[Lexer] > next token is CONSTANT(STRING)\n");

    uint32_t length = 0;
    LEXER_NEXT_STRING(if (c == '"') { source.next(); break; });

    std::string* str = new std::string(&source.peek() - length - 1, length);

    token = new Token(Token::CONSTANT, new Variable(&token->location, Variable::STRING, str), TokenLocation(location));

    MIDEBUG(2, "[Lexer] > CONSTANT(STRING) token created\n");
    return 1;

  /* Token::CONSTANT INTEGER */
  }else if (LEXER_IS_DIGIT(c) || c == '-')
  {
    MIDEBUG(2, "[Lexer] > next token is CONSTANT(INTEGER)\n");

    uint32_t length = 1;
    LEXER_NEXT_STRING(if (!LEXER_IS_DIGIT(c)) { break; });

    std::string* number = new std::string(&source.peek() - length, length);

    token = new Token(Token::CONSTANT, new Variable(&token->location, Variable::INTEGER, new int(std::stoi(*number))), TokenLocation(location));

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
      if (!tokenize_token(source, location, token, storage, NO_BREAK | flags))
	return 0;

      if (token == nullptr)
	continue;
      
      /* [Error] expected constant */
      if (token->type != Token::CONSTANT)
      {
	throw Exception(&token->location, EEXPECTED_CONSTANT, { });
      }

      elements->push_back(token->value.c);
    }

    MIDEBUG(2, "[Lexer] > [CONSTANT(LIST)] > found %lu elements\n", elements->size());

    token = new Token(Token::CONSTANT, new Variable(&token->location, Variable::LIST, elements), TokenLocation(location));

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
      if (!tokenize_token(source, location, token, storage, NO_BREAK | flags))
	return 0;

      if (token == nullptr)
	continue;

      if (token->type != expect)
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

    MIDEBUG(2, "[Lexer] > [CONSTANT(STRUCT)] > found %lu elements\n", st->size());
   
    token = new Token(Token::CONSTANT, new Variable(&token->location, Variable::STRUCT, st), TokenLocation(location));

    MIDEBUG(2, "[Lexer] > CONSTANT(STRUCT) token created\n");
    return 1;

  /* Token::CONSTANT REFERENCE */
  }else if (c == '$')
  {
    MIDEBUG(2, "[Lexer] > next token is CONSTANT(REFERENCE)\n");

    uint32_t length = 0;
    LEXER_NEXT_STRING(if (!LEXER_IS_NAME(c)) { break; });

    std::string* ref = new std::string(&source.peek() - length, length);

    token = new Token(Token::CONSTANT, new Variable(&token->location, Variable::REFERENCE, ref), TokenLocation(location));

    MIDEBUG(2, "[Lexer] > CONSTANT(REFERENCE) token created\n");
    return 1;
  }
  return 1;
}
