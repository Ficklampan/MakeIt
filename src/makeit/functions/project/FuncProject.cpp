#include "../Functions.hpp"
#include "../FuncUtils.hpp"

#include "../../Project.hpp"

static std::map<std::string, uint16_t> struct_project;

makeit::Function* makeit::function::make_project()
{
  return new Function({
      new StructArg({
	  StructArg::Arg("name", true, new Argument(Variable::STRING)),
	  StructArg::Arg("description", false, new Argument(Variable::STRING)),
	  StructArg::Arg("version", false, new Argument(Variable::STRING)),
	  StructArg::Arg("lang", false, new Argument(Variable::STRING)),
	  StructArg::Arg("cc", false, new Argument(Variable::STRING)),
	  StructArg::Arg("build", false, new Argument(Variable::STRING)),
	  StructArg::Arg("output", false, new Argument(Variable::STRING))
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
    else if (key.compare("lang") == 0)
      project->language = *value->as_string();
    else if (key.compare("cc") == 0)
      project->config.cc = *value->as_string();
    else if (key.compare("build") == 0)
      project->config.build = *value->as_string();
    else if (key.compare("output") == 0)
    {
      std::string &type = *value->as_string();
      if (type.compare("executable") == 0)
	project->config.output_type = BuildConfig::EXECUTABLE;
      else if (type.compare("static_library") == 0)
	project->config.output_type = BuildConfig::STATIC_LIBRARY;
      else if (type.compare("shared_library") == 0)
	project->config.output_type = BuildConfig::SHARED_LIBRARY;
      else
	throw Exception(value->location, EUNKNOWN_BUILD_TYPE, { type.c_str() });
    }
  }

  storage->variables["project"] = new Variable(nullptr, Variable::POINTER, project);
  return 1;
}
