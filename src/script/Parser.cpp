#include "Parser.hpp"

#include "Common.hpp"

#include <cstring>

static inline MI::Constant* PARSER_GET_CONSTANT(MI::Token* token, MI::Storage &storage)
{
  if (token->type == MI::Token::CONSTANT)
    return token->value.c;
  else if (token->type == MI::Token::LITERIAL)
    return storage.variables[*token->value.s];

  return nullptr;
}

int MI::Parser::parse(me::BasicIterator<Token*> &tokens, Storage &storage)
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
	Variable* var = storage.variables[*var_name];

	token = tokens.next();

	/* punctuator type */
	int p = *token->value.i;

	if (p == Token::EQUAL)
	{
	  if (var == nullptr)
	  {
	    var = new Variable(Variable::VOID, nullptr);
	    storage.variables[*var_name] = var;
	  }

	  /* Get next Constant / Variable */
	  token = tokens.next();
	  Constant* constant = PARSER_GET_CONSTANT(token, storage);

	  if (constant != nullptr)
	  {
	    var->type = token->value.c->type;
	    var->value = token->value.c->value;
	  }

	}else if (var != nullptr)
	{
	  /* Get next Constant / Variable */
	  token = tokens.next();
	  Constant* constant = PARSER_GET_CONSTANT(token, storage);

	  /* [Error] expected value after operator */
	  if (constant == nullptr)
	  {
	    printf(":: expected value after operator.\n");
	    return 0;
	  }

	  if (p == Token::PLUS_EQUAL)
	    *var+=*constant;

	/* [Error] Variable not found */
	}else
	{
	  printf(":: variable not found '%s'.\n", var_name->c_str());
	  return 0;
	}

      /* Function stuff */
      }else
      {
	Function* func = storage.functions[*token->value.s];

	/* [Error] Function not found */
	if (func == nullptr)
	{
	  printf(":: function not found '%s'.\n", token->value.s->c_str());
	  return 0;
	}

	std::vector<Constant*> args;
	if (!parse_args(tokens, storage, args))
	  return 0;

	char* info = nullptr;
	if (!func->execute(args, info))
	{
	  printError(info, &token->location);
	  return 0;
	}
      }
    }
  }

  return 1;
}

int MI::Parser::parse_args(me::BasicIterator<Token*> &tokens, Storage &storage, std::vector<Constant*> &args)
{
  while (tokens.hasNext())
  {
    Token* token = tokens.next();

    if (token->type == Token::BREAK)
      break;

    Constant* constant = PARSER_GET_CONSTANT(token, storage);

    /* [Error] expected value as a argument */
    if (constant == nullptr)
    {
      printf(":: expected value as a argument.\n");
      return 0;
    }

    args.push_back(constant);
  }
  return 1;
}
