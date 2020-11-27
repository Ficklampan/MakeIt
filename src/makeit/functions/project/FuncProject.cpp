#include "../Functions.hpp"
#include "../FuncUtils.hpp"

#include "../../Project.hpp"

static std::map<std::string, uint16_t> struct_project;

makeit::Function* makeit::function::make_project()
{
  return new Function({
      new StructArg({
	  StructArg::Arg("name", true, new Argument(Variable::STRING)),
	  StructArg::Arg("version", false, new Argument(Variable::STRING)),
	  StructArg::Arg("lang", false, new Argument(Variable::STRING)),
	  StructArg::Arg("cc", false, new Argument(Variable::STRING)),
	  StructArg::Arg("build", false, new Argument(Variable::STRING))
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
    else if (key.compare("version") == 0)
      project->version = *value->as_string();
    else if (key.compare("lang") == 0)
      project->language = *value->as_string();
    else if (key.compare("cc") == 0)
      project->config.cc = *value->as_string();
    else if (key.compare("build") == 0)
      project->config.build = *value->as_string();
  }

  storage->variables["project"] = new Variable(nullptr, Variable::POINTER, project);
  return 1;
}
