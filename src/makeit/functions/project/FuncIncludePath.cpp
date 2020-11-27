#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_include_path()
{
  return new Function({
      new ListArg(new Argument(Variable::STRING))
      }, exec_include_path);
}

int makeit::function::exec_include_path(void* ptr, std::vector<Variable*> &args)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE("project", makeit::Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    GET_STRINGS(v, project->config.include_paths, "-I");
  return 1;
}
