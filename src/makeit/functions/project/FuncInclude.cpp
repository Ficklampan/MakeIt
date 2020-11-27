#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_include()
{
  return new Function({
      new ListArg(new Argument(Variable::STRING))
      }, exec_include);
}

int makeit::function::exec_include(void* ptr, std::vector<Variable*> &args)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE("project", makeit::Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    GET_STRINGS(v, project->config.includes, "-i");
  return 1;
}
