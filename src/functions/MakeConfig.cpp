#include "Functions.hpp"

#include "Common.hpp"

#include "../script/Storage.hpp"

#include "../configs/GNUMake.hpp"

int MI::function::exec_makefile(void* ptr, std::vector<VariableRef*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::VariableRef::STRUCT);

  MI::Project* project = (MI::Project*) VARIABLE_STRUCT(storage->variables["project"])->value;

  std::string &filepath = VARIABLE_STRING(args.at(0))->value;
  std::string &compiler = VARIABLE_STRING(args.at(1))->value;
  me::File build_dir = VARIABLE_STRING(args.at(2))->value;

  std::vector<std::string*> flags;
  std::vector<std::string*> sources;
  std::vector<std::string*> headers;

  APPEND_STRINGS(args.at(3), flags);
  APPEND_STRINGS(args.at(4), sources);
  APPEND_STRINGS(args.at(5), headers);

  GNUMake_config(filepath, compiler, build_dir, flags, sources, headers, project);
  return 1;
}
