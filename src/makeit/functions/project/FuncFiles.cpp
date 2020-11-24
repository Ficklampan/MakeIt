#include "../Functions.hpp"
#include "../FuncUtils.hpp"

#include "../../Project.hpp"

makeit::Function* makeit::function::make_files()
{
  return new Function(1,
      new uint16_t[1]{
      0 | (Variable::LIST << 2)
      }, exec_files);
}

int makeit::function::exec_files(void* ptr, std::vector<Variable*> &args, char* &info)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  GET_STRINGS(args.at(0), project->config.sources);

  return 1;
}
