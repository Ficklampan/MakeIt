#include "Functions.hpp"

#include "Common.hpp"

#include "../System.hpp"

makeit::Function* makeit::function::make_system()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 1) | (Variable::LIST << 5)
      }, exec_system);
}
int makeit::function::exec_system(void* ptr, std::vector<Variable*> &args, char* &info)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  std::vector<std::string*> commands;

  for (Variable* v : args)
    APPEND_STRINGS(v, commands, storage);

  for (std::string* cmd : commands)
    makeit::system(cmd->c_str());

  return 1;
}