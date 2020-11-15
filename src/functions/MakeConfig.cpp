#include "Functions.hpp"

#include "Common.hpp"

#include "../script/Storage.hpp"

#include "../configs/GNUMake.hpp"

int MI::function::exec_makefile(void* ptr, std::vector<Constant*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::Constant::STRUCT, MI::Project*);

  std::string filepath = *args.at(0)->value.s;
  std::string compiler = *args.at(1)->value.s;
  me::File build_dir = *args.at(2)->value.s;

  std::vector<std::string*> flags;
  std::vector<std::string*> sources;
  std::vector<std::string*> headers;

  APPEND_STRINGS(args.at(3), flags);
  APPEND_STRINGS(args.at(4), sources);
  APPEND_STRINGS(args.at(5), headers);

  GNUMake_config(filepath, compiler, build_dir, flags, sources, headers, project);
  return 1;
}
