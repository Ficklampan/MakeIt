#include "../Functions.hpp"

#include "../Project.hpp"

int MI::function::project(void* ptr, std::vector<Constant*> &args)
{
  Project* project = (Project*) ptr;

  project->name = *args.at(0)->value.s;
  project->version = *args.at(1)->value.s;
  project->language = *args.at(2)->value.s;

  return 1;
}

int MI::function::source(void* ptr, std::vector<Constant*> &args)
{
  return 1;
}
