#include "../Functions.hpp"
#include "../FuncUtils.hpp"

#include "../../Project.hpp"

static std::map<std::string, uint16_t> struct_project;

makeit::Function* makeit::function::make_project()
{
  struct_project["name"] = 0 | (Variable::STRING << 2);
  struct_project["version"] = Variable::OPTIONAL | (Variable::STRING << 2);
  struct_project["lang"] = Variable::OPTIONAL | (Variable::STRING << 2);
  struct_project["cc"] = Variable::OPTIONAL | (Variable::STRING << 2);
  struct_project["build"] = Variable::OPTIONAL | (Variable::STRING << 2);

  return new Function(1,
      new uint16_t[1]{
      0 | (Variable::STRUCT << 2)
      }, exec_project);
}
int makeit::function::exec_project(void* ptr, std::vector<Variable*> &args, char* &info)
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
