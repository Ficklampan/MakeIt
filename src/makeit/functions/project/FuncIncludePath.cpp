#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_include_path()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 1) | (Variable::LIST << 5)
      }, exec_include_path);
}

int makeit::function::exec_include_path(void* ptr, std::vector<Variable*> &args, char* &info)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE(project, makeit::Variable::POINTER);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    APPEND_STRINGS(v, project->include_paths, storage);
  return 1;
}
