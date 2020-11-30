#include "../Functions.hpp"
#include "../FuncUtils.hpp"

#include "../../Project.hpp"

makeit::Function* makeit::function::make_project()
{
  return new Function({
      new StructArg({
	  StructArg::Arg("name", true, new Argument(Variable::STRING)),
	  StructArg::Arg("description", false, new Argument(Variable::STRING)),
	  StructArg::Arg("version", false, new Argument(Variable::STRING)),
	  })
      }, exec_project);
}

int makeit::function::exec_project(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  Project* project = new Project;

  Variable::v_struct* st = args.at(0)->as_struct();

  for (auto &[key, value] : *st)
  {
    if (key.compare("name") == 0)
      project->name = *value->as_string();
    else if (key.compare("description") == 0)
      project->description = *value->as_string();
    else if (key.compare("version") == 0)
      project->version = *value->as_string();
  }

  storage->variables["project"] = new Variable(nullptr, Variable::POINTER, project);
  return 1;
}
