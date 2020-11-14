#include "Parser.hpp"

#include "Common.hpp"

#include <cstring>

static inline int PARSER_FIX_STRING(MI::Token* token, std::string* str, std::string* &fixed, MI::Storage* storage)
{
  if (str->find("$(") == std::string::npos)
  {
    fixed = str;
    return 1;
  }

  fixed = new std::string;
  fixed->reserve(str->size() + 32);

  for (uint32_t i = 0; i < str->size(); i++)
  {
    char c = str->at(i);
    char next = i < str->size() - 1 ? str->at(i + 1) : '\0';

    if (c == '$' && next == '(')
    {
      i += 2;
      const char* begin = str->c_str() + i;

      uint32_t length = 0;
      for (; i < str->size(); i++)
      {
	if (str->at(i) == ')')
	  break;
	length++;
      }

      std::string var_name(begin, length);

      if (var_name.compare("DIR") == 0)
      {
	fixed->append(token->location.file->directoryPath());
      }else if (var_name.compare("FILE") == 0)
      {
	fixed->append(token->location.file->getPath());
      }else
      {
	MI::Variable* var = storage->variables[var_name];

	if (var == nullptr)
	{
	  printError(&token->location, "undefined variable '%s'", var_name.c_str());
	  return 0;
	}else
      	{
	  if (var->type == MI::Constant::LIST)
	  {
	    printError(&token->location, "unexpected reference type 'list'");
	    return 0;
	  }else if (var->type == MI::Constant::STRUCT)
	  {
	    printError(&token->location, "unexpected reference type 'struct'");
	    return 0;
	  }else if (var->type == MI::Constant::STRING)
      	  {
      	    fixed->append(*var->value.s);
      	  } /* TODO: */
      	}
      }

    }else
      fixed->push_back(c);
  }
  return 1;
}

static inline int PARSER_FIX_CONSTANT(MI::Token* token, MI::Constant* constant, MI::Storage* storage)
{
  if (constant != nullptr && constant->type == MI::Constant::STRING)
  {
    if (!PARSER_FIX_STRING(token, constant->value.s, constant->value.s, storage))
      return 0;
  }else if (constant != nullptr && constant->type == MI::Constant::LIST)
  {
    for (MI::Constant* c : *constant->value.l)
    {
      if (!PARSER_FIX_CONSTANT(token, c, storage))
	return 0;
    }
  }
  return 1;
}

static inline int PARSER_GET_CONSTANT(MI::Token* token, MI::Storage* storage, MI::Constant* &constant)
{
  if (token->type == MI::Token::CONSTANT)
    constant = token->value.c;
  else if (token->type == MI::Token::LITERIAL)
    constant = storage->variables[*token->value.s];

  if (constant != nullptr)
  {
    if (!PARSER_FIX_CONSTANT(token, constant, storage))
      return 0;
  }

  return 1;
}

int MI::Parser::parse(me::BasicIterator<Token*> &tokens, Storage* storage)
{
  while (tokens.hasNext())
  {
    Token* token = tokens.next();

    if (token->type == Token::BREAK)
      continue;

    if (token->type == Token::LITERIAL)
    {
      /* Variable stuff */
      if (tokens.peek()->type == Token::PUNCTUATOR)
      {
	std::string* var_name = token->value.s;
	Variable* var = storage->variables[*var_name];

	token = tokens.next();

	/* punctuator type */
	int p = *token->value.i;

	if (p == Token::EQUAL)
	{
	  if (var == nullptr)
	  {
	    var = new Variable(Variable::VOID, nullptr);
	    storage->variables[*var_name] = var;
	  }

	  /* Get next Constant / Variable */
	  token = tokens.next();
	  Constant* constant = nullptr;
	  if (!PARSER_GET_CONSTANT(token, storage, constant))
	    return 0;

	  if (constant != nullptr)
	  {
	    var->type = token->value.c->type;
	    var->value = token->value.c->value;
	  }

	}else if (var != nullptr)
	{
	  /* Get next Constant / Variable */
	  token = tokens.next();
	  Constant* constant = nullptr;
	  if (!PARSER_GET_CONSTANT(token, storage, constant))
	    return 0;

	  /* [Error] expected value after operator */
	  if (constant == nullptr)
	  {
	    printError(&token->location, "expected value after operator");
	    return 0;
	  }

	  if (p == Token::PLUS_EQUAL)
	    *var+=*constant;

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

	std::vector<Constant*> args;
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
	  Constant* arg = args.at(i);

	  bool match = false;

	  /* go through all possible argument types */
	  for (uint32_t j = 0; j < 5; j++)
	  {
	    uint8_t type = ((req_arg >> 1) >> (j * 3)) & 0x7;
	    if (type == 0)
	      break;
	    else if (type == Constant::VOID || type == arg->type)
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
	      expected_type.append(Constant::typeName((Constant::Type) type));
	      expected_type += '\'';
	    }

	    printError(&token->location, "expected type %s but found type '%s'", expected_type.c_str(), Constant::typeName(arg->type));
	    return 0;
	  }

	  if (req_arg & 0x1)
	    endless = true;
	  else if (!endless)
	    arg_index++;
	}

	char* info = nullptr;
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

int MI::Parser::parse_args(me::BasicIterator<Token*> &tokens, Storage* storage, std::vector<Constant*> &args)
{
  while (tokens.hasNext())
  {
    Token* token = tokens.next();

    if (token->type == Token::BREAK)
      break;

    Constant* constant = nullptr;
    if (!PARSER_GET_CONSTANT(token, storage, constant))
      return 0;

    /* [Error] expected value as a argument */
    if (constant == nullptr)
    {
      printError(&token->location, "expected value as a argument");
      return 0;
    }

    args.push_back(constant);
  }
  return 1;
}
