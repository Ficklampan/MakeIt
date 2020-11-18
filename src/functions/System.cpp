#include "Functions.hpp"

#include "Common.hpp"

#include "../System.hpp"

int MI::function::exec_system(void* ptr, std::vector<VariableRef*> &args, char* &info)
{
  std::vector<std::string*> commands;

  for (VariableRef* v : args)
    APPEND_STRINGS(v, commands);

  for (std::string* cmd : commands)
    MI::system(cmd->c_str());

  return 1;
}
