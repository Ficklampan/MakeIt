#include "Parser.hpp"
#include "../../Text.hpp"
#include "../Script.hpp"

#include "../../Config.hpp"
#include "Common.hpp"

#include <cstring>

extern makeit::Config config;

static inline int PARSER_FIX_STRING(makeit::Token* token, std::string &str, makeit::Storage* storage)
{
  bool paren = false;

  for (uint32_t i = 0; i < str.size(); i++)
  {
    char c = str.at(i);

    if (c == '$')
    {
      i++;

      uint32_t start = 0, end = 0;

      do {

	c = str.at(i);

	if (c == '(' || c == '{')
	{
	  paren = true;
	  start = i + 1;
	}else if (c == ')' || c == '}')
	{
	  paren = false;
	  end = i;
	}
	i++;

      }while (paren);

      if (end > start)
      {
	std::string ref_name(&str.at(0) + start, end - start);

	MIDEBUG(2, "[Parser] > referance found inside string '%s'\n", ref_name.c_str());

	if (ref_name.compare("DIR") == 0) str.replace(start - 2, end + 1, token->location.file->directoryPath());
	else if (ref_name.compare("FILE") == 0) str.replace(start - 2, end + 1, token->location.file->getPath());
	else
	{
	  makeit::Variable* var = storage->variables[ref_name];
	  if (var != nullptr)
	  {
	    if (var->type == makeit::Variable::STRING)
	      str.replace(start - 2, end + 1, *var->as_string());
	  }
	}
      }

    }
  }
  return 1;
}

static inline int PARSER_FIX_VARIABLE(makeit::Token* token, makeit::Variable* &variable, makeit::Storage* storage)
{
  if (variable == nullptr)
    return 1;

  /* get the Variable from reference */
  if (variable->type == makeit::Variable::REFERENCE)
  {
    const std::string* ref_name = variable->as_reference();
    variable = storage->variables[*ref_name];

    if (variable == nullptr)
    {
      throw makeit::Exception(&token->location, makeit::EUNDEFINED_VARIABLE, { ref_name->c_str() });
    }
  }

  if (variable->type == makeit::Variable::STRING)
  {
    if (!PARSER_FIX_STRING(token, *variable->as_string(), storage))
      return 0;
  }else if (variable->type == makeit::Variable::LIST)
  {
    makeit::Variable::v_list* list = variable->as_list();
    for (makeit::Variable* v : *list)
    {
      if (!PARSER_FIX_VARIABLE(token, v, storage))
	return 0;
    }
  }else if (variable->type == makeit::Variable::STRUCT)
  {
    makeit::Variable::v_struct* st = variable->as_struct();
    for (auto &[key, value] : *st)
    {
      if (!PARSER_FIX_VARIABLE(token, value, storage))
	return 0;
    }
  }
  return 1;
}



int makeit::Parser::parse_tokens(me::BasicIterator<Token*> &tokens, Storage* storage, uint8_t flags)
{
  while (tokens.hasNext())
  {
    Token* token = tokens.next();

    if (!parse_token(token, tokens, storage, flags))
      return 0;
  }
  return 1;
}

int makeit::Parser::parse_token(Token* token, me::BasicIterator<Token*> &tokens, Storage* storage, uint8_t flags)
{
  MIDEBUG(2, "[Parser] > parsing %s token\n", Token::type_name(token->type));

  if ((flags & NO_BREAK) == 0 && token->type == Token::BREAK)
    return 1;

  if (token->type == Token::LITERIAL)
  {
    if (!parse_literial(token, tokens, storage, flags))
      return 0;
  }else if (token->type == Token::IF)
  {
    if (!parse_statement(token, tokens, storage, flags))
      return 0;
  }else if (token->type == Token::CALL)
  {
    if (!parse_function(token, tokens, storage, flags))
      return 0;
  }else if ((flags & NO_WARNING) == 0)
  {
    print_warning(&token->location, get_warning(WWILD_TOKEN));
  }
  return 1;
}

int makeit::Parser::get_args(me::BasicIterator<Token*> &tokens, Storage* storage, std::vector<Variable*> &args, uint8_t flags)
{
  while (tokens.hasNext())
  {
    Token* token = tokens.next();

    if (token->type == Token::BREAK)
      break;

    Variable* variable = nullptr;
    get_variable(token, tokens, storage, variable, flags);

    args.push_back(variable);
  }
  return 1;
}

int makeit::Parser::get_variable(Token* token, me::BasicIterator<Token*> &tokens, Storage* storage, Variable* &variable, uint8_t flags)
{
  if (token->type == Token::CONSTANT)
    variable = token->value.c;
  else if (token->type == Token::WILDCARD)
  {
    bool found = storage->variables[token->value.w->value] != nullptr;
    variable = new Variable(&token->location, Variable::INTEGER, new int(found));
  }

  if (variable == nullptr && (flags & NO_ERROR) == 0)
  {
    throw Exception(&token->location, EEXPECTED_VALUE, { });
  }else if (variable != nullptr)
  {
    if (!PARSER_FIX_VARIABLE(token, variable, storage))
      return 0;
    return 1;
  }
  return 1;
}
