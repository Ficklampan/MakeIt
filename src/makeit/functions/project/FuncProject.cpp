#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_project()
{
  return new Function(3,
      new uint16_t[3]{
      0 | (Variable::STRING << 1),
      0 | (Variable::STRING << 1),
      0 | (Variable::STRING << 1)
      }, exec_project);
}
int makeit::function::exec_project(void* ptr, std::vector<Variable*> &args, char* &info)
{
  Storage* storage = (Storage*) ptr;

  Project* project = new Project;
  project->name = *args.at(0)->as_string();
  project->version = *args.at(1)->as_string();
  project->language = *args.at(2)->as_string();

  storage->variables["project"] = new Variable(Variable::POINTER, project);

  return 1;
}
