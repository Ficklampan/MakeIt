#include "../Functions.hpp"
#include "../FuncUtils.hpp"

#include "../../Project.hpp"

makeit::Function* makeit::function::make_cflags()
{
  return new Function({
      new ListArg(new Argument(Variable::STRING))
      }, exec_cflags);
}

int makeit::function::exec_cflags(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  GET_STRINGS(args.at(0), project->config.cflags);

  return 1;
}

makeit::Function* makeit::function::make_lflags()
{
  return new Function({
      new ListArg(new Argument(Variable::STRING))
      }, exec_lflags);
}

int makeit::function::exec_lflags(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  GET_STRINGS(args.at(0), project->config.lflags);

  return 1;
}
