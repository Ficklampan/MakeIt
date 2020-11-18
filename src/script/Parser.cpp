#include "Parser.hpp"

#include "../Config.hpp"

#include "Common.hpp"

#include <cstring>

extern Config config;

static inline int PARSER_FIX_STRING(MI::Token* token, MI::VarString* str, MI::Storage* storage)
{
  bool paren = false;

  for (uint32_t i = 0; i < str->value.size(); i++)
  {
    char c = str->value.at(i);

    if (c == '$')
    {
      i++;

      uint32_t start = 0, end = 0;

      do {

	c = str->value.at(i);

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
	std::string ref_name(&str->value.at(0) + start, end - start);

	MIDEBUG(2, "[Parser] > referance found inside string '%s'\n", ref_name.c_str());

	if (ref_name.compare("DIR") == 0) str->value.replace(start - 2, end + 1, token->location.file->directoryPath());
	else if (ref_name.compare("FILE") == 0) str->value.replace(start - 2, end + 1, token->location.file->getPath());
	else
	{
	  MI::VariableRef* var = storage->variables[ref_name];
	  if (var != nullptr)
	  {
	    if (var->type == MI::VariableRef::STRING)
	      str->value.replace(start - 2, end + 1, VARIABLE_STRING(var)->value);
	  }
	}
      }

    }
  }
  return 1;
}

static inline int PARSER_FIX_VARIABLE(MI::Token* token, MI::VariableRef* variable, MI::Storage* storage)
{
  if (variable != nullptr && variable->type == MI::VariableRef::STRING)
  {
    if (!PARSER_FIX_STRING(token, (MI::VarString*) variable, storage))
      return 0;
  }else if (variable != nullptr && variable->type == MI::VariableRef::LIST)
  {
    MI::VarList* list = (MI::VarList*) variable;
    for (MI::VariableRef* v : list->value)
    {
      if (!PARSER_FIX_VARIABLE(token, v, storage))
	return 0;
    }
  }
  return 1;
}

static inline int PARSER_GET_VARIABLE(MI::Token* token, MI::Storage* storage, MI::VariableRef* &variable)
{
  if (token->type == MI::Token::CONSTANT)
    variable = token->value.c;
  else if (token->type == MI::Token::LITERIAL)
    variable = storage->variables[*token->value.s];

  if (variable != nullptr)
  {
    if (!PARSER_FIX_VARIABLE(token, variable, storage))
      return 0;
  }

  return 1;
}

int MI::Parser::parse(me::BasicIterator<Token*> &tokens, Storage* storage)
{
  while (tokens.hasNext())
  {
    Token* token = tokens.next();

    MIDEBUG(2, "[Parser] > parsing %s token\n", Token::typeName(token->type));

    if (token->type == Token::BREAK)
      continue;

    if (token->type == Token::LITERIAL)
    {
      /* Variable stuff */
      if (tokens.peek()->type == Token::PUNCTUATOR)
      {
	std::string* var_name = token->value.s;
	VariableRef* var = storage->variables[*var_name];

	token = tokens.next();

	/* punctuator type */
	int p = *token->value.i;

	if (p == Token::EQUAL)
	{
	  /* Get next Constant / Variable */
	  token = tokens.next();
	  VariableRef* var2 = nullptr;
	  if (!PARSER_GET_VARIABLE(token, storage, var2))
	    return 0;

	  if (var2 != nullptr)
	  {
	    if (var == nullptr)
	    {
	      var = VariableRef::copy(var2);
	      storage->variables[*var_name] = var;
	    }else
	    {
	      if (!var->assign(var2))
	      {
		printError(&token->location, "could not assign type '%s' to '%s'", 
		    VariableRef::typeName(var->type), 
		    VariableRef::typeName(var2->type));
		return 0;
	      }
	    }
	  }

	}else if (var != nullptr)
	{
	  /* Get next Constant / Variable */
	  token = tokens.next();
	  VariableRef* variable = nullptr;
	  if (!PARSER_GET_VARIABLE(token, storage, variable))
	    return 0;

	  /* [Error] expected value after operator */
	  if (variable == nullptr)
	  {
	    printError(&token->location, "expected value after operator");
	    return 0;
	  }

	  if (p == Token::PLUS_EQUAL)
	    *var+=variable;

	/* [Error] Variable not found */
	}else
	{
	  printError(&token->location, "variable not found '%s'", var_name->c_str());
	  return 0;
	}

      /* Function stuff */
      }else
      {
	Function* func = storage->functions[*token->value.s];

	/* [Error] Function not found */
	if (func == nullptr)
	{
	  printError(&token->location, "function not found '%s'", token->value.s->c_str());
	  return 0;
	}

	std::vector<VariableRef*> args;
	args.reserve(5);
	if (!parse_args(tokens, storage, args))
	  return 0;

	/* [Error] too few arguments */
	if (args.size() < func->argc)
	{
	  printError(&token->location, "too few arguments");
	  return 0;
	}

	/* check if arguments matches the required arguments */
	bool endless = false;
	uint32_t arg_index = 0;
	for (uint32_t i = 0; i < args.size(); i++)
	{
	  /* [Error] too many arguments */
	  if (!endless && arg_index >= func->argc)
	  {
	    printError(&token->location, "too many arguments");
	    return 0;
	  }

	  uint16_t req_arg = func->argv[arg_index];
	  VariableRef* arg = args.at(i);

	  bool match = false;

	  /* go through all possible argument types */
	  for (uint32_t j = 0; j < 5; j++)
	  {
	    uint8_t type = ((req_arg >> 1) >> (j * 3)) & 0x7;
	    if (type == 0)
	      break;
	    else if (type == VariableRef::VOID || type == arg->type)
	    {
	      match = true;
	      break;
	    }
	  }

	  /* [Error] no matching type */
	  if (!match)
	  {
	    std::string expected_type;
	    for (uint32_t j = 0; j < 5; j++)
	    {
	      uint8_t type = ((req_arg >> 1) >> (j * 3)) & 0x7;

	      if (type == 0)
		break;

	      if (j > 0)
		expected_type += '/';

	      expected_type += '\'';
	      expected_type.append(VariableRef::typeName((VariableRef::Type) type));
	      expected_type += '\'';
	    }

	    printError(&token->location, "expected type %s but found type '%s'", 
		expected_type.c_str(), 
		VariableRef::typeName(arg->type));
	    return 0;
	  }

	  if (req_arg & 0x1)
	    endless = true;
	  else if (!endless)
	    arg_index++;
	}

	char* info = nullptr;

	MIDEBUG(2, "[Parser] > calling function '%s'\n", token->value.s->c_str());
	if (!func->exec(storage, args, info))
	{
	  if (info != nullptr && strlen(info) > 0)
	    printError(&token->location, info);
	  return 0;
	}
      }
    }
  }

  return 1;
}

int MI::Parser::parse_args(me::BasicIterator<Token*> &tokens, Storage* storage, std::vector<VariableRef*> &args)
{
  while (tokens.hasNext())
  {
    Token* token = tokens.next();

    if (token->type == Token::BREAK)
      break;

    VariableRef* variable = nullptr;
    if (!PARSER_GET_VARIABLE(token, storage, variable))
      return 0;

    /* [Error] expected value as a argument */
    if (variable == nullptr)
    {
      printError(&token->location, "expected value as a argument");
      return 0;
    }

    args.push_back(variable);
  }
  return 1;
}
