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
    throw Exception(&token->location, EUNDEFINED_FUNCTION, { token->value.s->c_str() });
  }

  std::vector<Variable*> args;
  args.reserve(5);
  get_args(tokens, storage, args, flags);

  if (func->args.size() == 0 && args.size() > 0)
    throw Exception(&token->location, ETOO_MANY_ARGS, { (uint32_t) func->args.size(), (uint32_t) args.size() });

  uint32_t min_args = 0;
  for (const Argument* arg : func->args)
  {
    if (arg->is_optional())
      break;
    min_args++;
    if (arg->is_endless())
      break;
  }

  if (args.size() < min_args)
    throw Exception(&token->location, ETOO_FEW_ARGS, { (uint32_t) func->args.size(), (uint32_t) args.size() });

  /* check if arguments matches the required arguments */
  uint32_t arg_index = 0;
  for (uint32_t i = 0; i < args.size(); i++)
  {
    if (arg_index >= func->args.size())
      throw Exception(&token->location, ETOO_MANY_ARGS, { (uint32_t) func->args.size(), (uint32_t) args.size() });

    const Argument* arg = func->args.at(arg_index);
    try {
      arg->match(args.at(i));
    }catch (const Exception<TokenLocation> &e)
    {
      throw e;
    }

    if (!arg->is_endless())
      arg_index++;
  }

  MIDEBUG(2, "[Parser] > calling function '%s'\n", token->value.s->c_str());
  try {
    func->exec(storage, args);
  }catch (const Exception<TokenLocation*> &e)
  {
    throw e;
  }
  return 1;
}
