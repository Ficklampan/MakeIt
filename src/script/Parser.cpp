#include "Parser.hpp"

#include "Common.hpp"

#include "../Config.hpp"
#include "../Common.hpp"

#include <cstring>

extern Config config;

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
    std::string* ref_name = variable->as_reference();
    variable = storage->variables[*ref_name];

    if (variable == nullptr)
    {
      printError(token->location, "undefined variable '%s'", ref_name->c_str());
      return 0;
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

static inline int PARSER_GET_VARIABLE(makeit::Token* token, makeit::Storage* storage, makeit::Variable* &variable)
{
  if (token->type == makeit::Token::CONSTANT)
    variable = token->value.c;

  if (variable != nullptr)
  {
    if (!PARSER_FIX_VARIABLE(token, variable, storage))
      return 0;
  }

  return 1;
}

int makeit::Parser::parse(me::BasicIterator<Token*> &tokens, Storage* storage)
{
  while (tokens.hasNext())
  {
    Token* token = tokens.next();

    MIDEBUG(2, "[Parser] > parsing %s token\n", Token::type_name(token->type));

    if (token->type == Token::BREAK)
      continue;

    /* Variable stuff */
    if (token->type == Token::LITERIAL)
    {
      std::string* name = token->value.s;


      if (tokens.peek()->type == Token::PUNCTUATOR)
      {
	token = tokens.next();
	Variable* variable_a = storage->variables[*name];
	
	/* punctuator type */
	int p = *token->value.i;
	token = tokens.next();

	/* Get next Constant / Variable */
	Variable* variable_b = nullptr;
	if (!PARSER_GET_VARIABLE(token, storage, variable_b))
	  return 0;

	/* [Error] expected value after operator */
	if (variable_b == nullptr)
	{
	  printError(token->location, "expected value after operator");
	  return 0;
	}

	if (p == Token::EQUAL)
	{
	  if (variable_a == nullptr)
	  {
	    variable_a = new Variable(variable_b->type, nullptr);
	    storage->variables[*name] = variable_a;
	  }else
	    variable_a->type = variable_b->type;
	  variable_a->value = variable_b->value;
	}

	/* [Error] undefined variable */
	if (variable_a == nullptr)
	{
	  printError(token->location, "undefined variable '%s'", name->c_str());
	  return 0;
	}else if (p == Token::PLUS_EQUAL)
	  (*variable_a) += variable_b;

      /* [Error] found identifier in the wild */
      }else
      {
	printError(token->location, "found identifier in the wild");
	return 0;
      }
    }

    /* Function stuff */
    if (token->type == Token::CALL)
    {
      Function* func = storage->functions[*token->value.s];

      /* [Error] Function not found */
      if (func == nullptr)
      {
        printError(token->location, "function not found '%s'", token->value.s->c_str());
        return 0;
      }

      std::vector<Variable*> args;
      args.reserve(5);
      if (!parse_args(tokens, storage, args))
        return 0;

      /* [Error] too few arguments */
      if (args.size() < func->argc)
      {
        printError(token->location, "too few arguments");
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
          printError(token->location, "too many arguments");
          return 0;
        }

        uint16_t req_arg = func->argv[arg_index];
	Variable* arg = args.at(i);

        bool match = false;

        /* go through all possible argument types */
        for (uint32_t j = 0; j < 3; j++)
        {
          uint8_t type = ((req_arg >> 1) >> (j * 4)) & 0xF;
          if (type == 0)
            break;
          else if (type == Variable::VOID || type == arg->type)
          {
            match = true;
            break;
          }
        }

        /* [Error] no matching type */
        if (!match)
        {
          std::string expected_type;
          for (uint32_t j = 0; j < 3; j++)
          {
            uint8_t type = ((req_arg >> 1) >> (j * 4)) & 0xF;

            if (type == 0)
      	break;

            if (j > 0)
      	expected_type += '/';

            expected_type += '\'';
            expected_type.append(Variable::type_name((Variable::Type) type));
            expected_type += '\'';
          }

        printError(token->location, "expected type %s at arg[%u] but found type '%s'", 
      	expected_type.c_str(), 
	i,
      	Variable::type_name(arg->type));
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
          printError(token->location, info);
        return 0;
      }
    }
  }

  return 1;
}

int makeit::Parser::parse_args(me::BasicIterator<Token*> &tokens, Storage* storage, std::vector<Variable*> &args)
{
  while (tokens.hasNext())
  {
    Token* token = tokens.next();

    if (token->type == Token::BREAK)
      break;

    Variable* variable = nullptr;
    if (!PARSER_GET_VARIABLE(token, storage, variable))
      return 0;

    /* [Error] expected value as a argument */
    if (variable == nullptr)
    {
      printError(token->location, "expected value as a argument");
      return 0;
    }

    args.push_back(variable);
  }
  return 1;
}
