#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../System.hpp"

makeit::Function* makeit::function::make_system()
{
  return new Function({
      new Argument(Variable::STRING, Argument::ENDLESS)
      }, exec_system);
}

int makeit::function::exec_system(void* ptr, std::vector<Variable*> &args)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  std::vector<std::string> commands;

  for (Variable* v : args)
    GET_STRINGS(v, commands);

  for (const std::string &cmd : commands)
    makeit::system(cmd.c_str());
  return 1;
}
