#include "Parser.hpp"
#include "../../Text.hpp"
#include "../Script.hpp"

#include "../../Config.hpp"

static inline int GET_BOOLEAN(me::BasicIterator<makeit::Token*> &tokens, makeit::Storage* storage, bool &out, uint8_t flags)
{
#define SET_OUT(b) { if (invert) out = !(b); else out = (b); }
  uint32_t i = 0;
  makeit::Token* token = nullptr;
  bool invert = false;
  while (tokens.hasNext())
  {
    token = tokens.peek();

    makeit::Variable* variable_a = nullptr;
    if (!makeit::Parser::get_variable(token, tokens, storage, variable_a, flags | makeit::Parser::NO_ERROR))
      return 0;

    if (variable_a != nullptr)
    {
      token = tokens.next();

      /* if next token is Token::PUNCTUATOR */
      if (tokens.peek()->type == makeit::Token::PUNCTUATOR)
      {
	token = tokens.next();
	int p = *token->value.i;

	token = tokens.next();

	/* get variable_b */
	makeit::Variable* variable_b = nullptr;
	if (!makeit::Parser::get_variable(token, tokens, storage, variable_b, flags))
	  return 0;

	/* check if variable equals variable_b */
	if (p == makeit::Token::EQUAL_EQUAL)
	{
	  SET_OUT((*variable_a) == variable_b);
	}

      /* if variable is integer/boolean */
      }else if (variable_a->type == makeit::Variable::INTEGER)
      {
	SET_OUT(*variable_a->as_integer() > 0); /* > 0 is true */
      }

    }else if (token->type == makeit::Token::PUNCTUATOR)
    {
      token = tokens.next();

      int p = *token->value.i;

      if (p == makeit::Token::AND_AND || p == makeit::Token::VBAR_VBAR)
      {
	bool b = false;
      	if (!GET_BOOLEAN(tokens, storage, b, flags))
      	  return 0;

      	if (p == makeit::Token::AND_AND)
      	{
      	  SET_OUT(out && b);
      	}else if (p == makeit::Token::VBAR_VBAR)
      	{
      	  SET_OUT(out || b);
      	}

      }else if (p == makeit::Token::EXCLMARK)
	invert = true;
    }else
      break;

    i++;
  }

  /* no token matched */
  /* [Error] expected expression */
  if (i == 0)
  {
    throw makeit::Exception(token == nullptr ? tokens.last()->location : token->location, makeit::EEXPECTED_EXPRESSION, { });
  }

#undef SET_OUT
  return 1;
}

static inline int PARSER_PARSE_SCOPE(bool b, me::BasicIterator<makeit::Token*> &tokens, makeit::Storage* storage, uint8_t flags)
{
  bool sleep = false;
  while (tokens.hasNext())
  {
    makeit::Token* token = tokens.next();

    if (sleep &&
	token->type != makeit::Token::ELSE &&
	token->type != makeit::Token::END)
      continue;
    sleep = false;

    if (token->type == makeit::Token::ELSE)
    {
      /* can only execute if the last if statement was false */
      if (b)
      {
	sleep = true;
	continue;
      }

      while (tokens.hasNext())
      {
	token = tokens.next();

	if (token->type == makeit::Token::END)
	  break;

	if (!makeit::Parser::parse_token(token, tokens, storage, flags))
	  return 0;
      }
    }

    if (token->type == makeit::Token::END)
      break;

    if (b && !makeit::Parser::parse_token(token, tokens, storage, flags))
      return 0;
  }
  return 1;
}

int makeit::Parser::parse_statement(Token* token, me::BasicIterator<Token*> &tokens, Storage* storage, uint8_t flags)
{
  bool b = false;
  if (!GET_BOOLEAN(tokens, storage, b, flags))
    return 0;

  token = tokens.next();

  /* [Error] expected token */
  if (token == nullptr || token->type != Token::THEN)
  {
    throw Exception(tokens.last()->location, EEXPECTED_TOKEN, { Token::type_name(Token::THEN) });
  }

  /* execute stuff under the if statement */
  if (!PARSER_PARSE_SCOPE(b, tokens, storage, flags))
    return 0;
  return 1;
}
