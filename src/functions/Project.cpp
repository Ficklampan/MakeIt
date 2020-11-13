#include "../Functions.hpp"

#include "../script/Storage.hpp"
#include "../Project.hpp"

MI::function::Project::Project()
{
  desc = "";
  args = ArgBuilder()
    .type(MI::Constant::STRING)
    .type(MI::Constant::STRING)
    .type(MI::Constant::STRING)
    .build();
}

int MI::function::Project::execute(void* ptr, std::vector<Constant*> &args, char* &info)
{
  if (args.size() == 0 || args.size() > 3)
  {
    info = (char*) (args.size() > 3 ? "too many arguments" : "expected 1 or more arguements");
    return 0;
  }

  MI::Storage* storage = (MI::Storage*) ptr;

  MI::Project* project = new MI::Project;
  project->name = *args.at(0)->value.s;
  project->version = *args.at(1)->value.s;
  project->language = *args.at(2)->value.s;

  storage->variables["project"] = new MI::Variable(MI::Constant::STRUCT, project);

  return 1;
}

MI::function::Source::Source()
{
  desc = "";
  args = ArgBuilder().type({MI::Constant::STRING, MI::Constant::LIST}).endless();
}

int MI::function::Source::execute(void* ptr, std::vector<Constant*> &args, char* &info)
{
  return 1;
}
