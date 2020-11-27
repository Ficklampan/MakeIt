#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_library()
{
  return new Function({
      new ListArg(new Argument(Variable::STRING))
      }, exec_library);
}

int makeit::function::exec_library(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    GET_STRINGS(v, project->config.libraries, "-l");
  return 1;
}
