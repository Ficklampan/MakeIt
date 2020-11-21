#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_library()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 1) | (Variable::LIST << 5)
      }, exec_library);
}

int makeit::function::exec_library(void* ptr, std::vector<Variable*> &args, char* &info)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE(project, Variable::POINTER);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    APPEND_STRINGS(v, project->libraries, storage);

  return 1;
}
