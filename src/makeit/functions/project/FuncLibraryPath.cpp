#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_library_path()
{
  return new Function({
      new ListArg(new Argument(Variable::STRING))
      }, exec_library_path);
}

int makeit::function::exec_library_path(void* ptr, std::vector<Variable*> &args)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  Variable::v_list &list = *args.at(0)->as_list();

  for (const Variable* var : list)
  {
    const std::string &lib = *var->as_string();
    project->config.library_paths.push_back(lib);
  }

  return 1;
}


