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
    throw Exception(token->location, EUNDEFINED_FUNCTION, { token->value.s->c_str() });
  }

  std::vector<Variable*> args;
  args.reserve(5);
  if (!get_args(tokens, storage, args, flags))
    return 0;

  /* [Error] too few arguments */
  if (args.size() < func->argc)
  {
    throw Exception(token->location, ETOO_FEW_ARGS, { });
  }

  /* check if arguments matches the required arguments */
  bool endless = false;
  uint32_t arg_index = 0;
  for (uint32_t i = 0; i < args.size(); i++)
  {
    /* [Error] too many arguments */
    if (!endless && arg_index >= func->argc)
    {
      throw Exception(token->location, ETOO_MANY_ARGS, { });
    }

    uint16_t req_arg = func->argv[arg_index];
    Variable* arg = args.at(i);

    bool match = false;

    /* go through all possible argument types */
    for (uint32_t j = 0; j < 3; j++)
    {
      uint8_t type = ((req_arg >> 2) >> (j * 4)) & 0xF;
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
        uint8_t type = ((req_arg >> 2) >> (j * 4)) & 0xF;

        if (type == 0)
	  break;

        if (j > 0)
	  expected_type += '/';

        expected_type.append(Variable::type_name((Variable::Type) type));
      }

      throw Exception(token->location, EEXPECTED_TYPE_AT_ARG, { expected_type.c_str(), i, Variable::type_name(arg->type) });
    }

    if (req_arg & Variable::ENDLESS)
      endless = true;
    else if (!endless)
      arg_index++;
  }

  char* info = nullptr;

  MIDEBUG(2, "[Parser] > calling function '%s'\n", token->value.s->c_str());
  try {
    func->exec(storage, args, info);
  }catch (const Exception<int> &e)
  {
    TokenLocation* location = e.get_location() >= 0 ? args.at(e.get_location())->location : &token->location;
    throw Exception(location == nullptr ? token->location : *location, e.get_err(), e.get_args());
  }
  return 1;
}
