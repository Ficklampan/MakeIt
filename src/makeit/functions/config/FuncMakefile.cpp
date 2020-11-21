#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"
#include "../../configs/GNUMake.hpp"

makeit::Function* makeit::function::make_makefile()
{
  return new Function(6,
      new uint16_t[6]{
      0 | (Variable::STRING << 1),
      0 | (Variable::STRING << 1),
      0 | (Variable::STRING << 1),
      0 | (Variable::STRING << 1) | (Variable::LIST << 5),
      0 | (Variable::LIST << 1),
      0 | (Variable::LIST << 1)
      }, exec_makefile);
}

int makeit::function::exec_makefile(void* ptr, std::vector<Variable*> &args, char* &info)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE(project, Variable::POINTER);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  std::string filepath = *args.at(0)->as_string();
  std::string compiler = *args.at(1)->as_string();
  me::File build_dir = *args.at(2)->as_string();

  std::vector<std::string*> flags;
  std::vector<std::string*> sources;
  std::vector<std::string*> headers;

  APPEND_STRINGS(args.at(3), flags, storage);
  APPEND_STRINGS(args.at(4), sources, storage);
  APPEND_STRINGS(args.at(5), headers, storage);

  GNUMake_config(filepath, compiler, build_dir, flags, sources, headers, project);
  return 1;
}
