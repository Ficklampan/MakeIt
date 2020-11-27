#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_library()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 2) | (Variable::LIST << 6)
      }, exec_library);
}

int makeit::function::exec_library(void* ptr, std::vector<Variable*> &args, char* &info)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    GET_STRINGS(v, project->config.libraries, "-l");
  return 1;
}
