#include "Parser.hpp"
#include "../../Text.hpp"
#include "../Script.hpp"

#include "../../Config.hpp"

#include <cstring>

extern makeit::Config config;

int makeit::Parser::parse_function(Token* token, me::BasicIterator<Token*> &tokens, Storage* storage, uint8_t flags)
{
  Function* func = storage->functions[*token->value.s];

  /* [Error] Function not found */
  if (func == nullptr)
  {
    printError(token->location, Eundefined_function, token->value.s->c_str());
    return 0;
  }

  std::vector<Variable*> args;
  args.reserve(5);
  if (!get_args(tokens, storage, args, flags))
    return 0;

  /* [Error] too few arguments */
  if (args.size() < func->argc)
  {
    printError(token->location, Etoo_few_args);
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
      printError(token->location, Etoo_many_args);
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

    printError(token->location, Eexpected_type_at_arg, 
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
  return 1;
}
