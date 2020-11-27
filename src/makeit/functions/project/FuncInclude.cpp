#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_include()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 2) | (Variable::LIST << 6)
      }, exec_include);
}

int makeit::function::exec_include(void* ptr, std::vector<Variable*> &args, char* &info)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE("project", makeit::Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    GET_STRINGS(v, project->config.includes, "-i");
  return 1;
}
