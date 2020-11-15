#include "Functions.hpp"

#include "Common.hpp"

#include "../System.hpp"

int MI::function::exec_system(void* ptr, std::vector<Constant*> &args, char* &info)
{
  std::vector<std::string*> commands;

  for (Constant* c : args)
    APPEND_STRINGS(c, commands);

  for (std::string* cmd : commands)
    MI::system(cmd->c_str());

  return 1;
}
