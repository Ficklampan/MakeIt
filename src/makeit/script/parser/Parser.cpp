#include "Parser.hpp"
#include "../../Text.hpp"
#include "../Script.hpp"

#include "../../Config.hpp"
#include "Common.hpp"
#include "Util.hpp"

#include <cstring>

extern makeit::Config config;

static inline int PARSER_FIX_STRING(makeit::Token* token, std::string &str, makeit::Storage* storage)
{
  uint32_t index = 0;
  while (index < str.size())
  {
    if (str.at(index) == '$' && index + 3 < str.size())
    {
      uint32_t start = index;

      index++;
      char next = str.at(index);
      index++;

      uint32_t name_start = index;

      if (next == '(' || next == '{')
      {
	uint32_t end = 0;
	for (uint32_t i = name_start; i < str.size(); i++)
	{
	  char c = str.at(i);
	  if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'))
	  {
	    if (c == ')' || c == '}')
	      end = i;
	    break;
	  }
	}

	if (end == 0)
	  continue;

	std::string_view name(str.c_str() + name_start, end - name_start);
	std::string value;
	if (name.compare("DIR") == 0)
	  value = token->location.file->directory_path();
	else if (name.compare("FILE") == 0)
	  value = token->location.file->get_path();
	else
	{
	  for (auto const &[var_name, var] : storage->variables)
	  {
	    /* skip every variable that is not a string */
	    if (var->type != makeit::Variable::STRING)
	      continue;

	    if (name.compare(var_name) == 0)
	    {
	      value = *var->as_string();
	      break;
	    }
	  }
	}

	str.replace(start, end - start + 1, value);
      }
    }
    index++;
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



int makeit::Parser::parse_tokens(me::Iterator<Token*> &tokens, Storage* storage, uint8_t flags)
{
  while (tokens.has_next())
  {
    Token* token = tokens.next();

    if (!parse_token(token, tokens, storage, flags))
      return 0;
  }
  return 1;
}

int makeit::Parser::parse_token(Token* token, me::Iterator<Token*> &tokens, Storage* storage, uint8_t flags)
{
  MIDEBUG(2, "[Parser] > parsing %s token\n", Token::type_name(token->type));

  if ((flags & NO_BREAK) == 0 && token->type == Token::BREAK)
    return 1;

  if (token->type == Token::LITERIAL)
  {
    if (!parse_literial(token, tokens, storage, flags))
      return 0;
  }else if (token->type == Token::FOREACH || token->type == Token::IF)
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

int makeit::Parser::get_args(me::Iterator<Token*> &tokens, Storage* storage, std::vector<Variable*> &args, uint8_t flags)
{
  while (tokens.has_next())
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

int makeit::Parser::get_variable(Token* token, me::Iterator<Token*> &tokens, Storage* storage, Variable* &variable, uint8_t flags)
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
