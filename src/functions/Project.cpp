#include "Functions.hpp"

#include "Common.hpp"

#include "../Project.hpp"

#include "../script/Script.hpp"

#include <climits>

int MI::function::exec_project(void* ptr, std::vector<Constant*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  MI::Project* project = new MI::Project;
  project->name = *args.at(0)->value.s;
  project->version = *args.at(1)->value.s;
  project->language = *args.at(2)->value.s;

  storage->variables["project"] = new MI::Variable(MI::Constant::STRUCT, project);

  return 1;
}

int MI::function::exec_library(void* ptr, std::vector<Constant*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::Constant::STRUCT, MI::Project*);

  for (Constant* c : args)
    APPEND_STRINGS(c, project->libraries);

  return 1;
}

int MI::function::exec_library_path(void* ptr, std::vector<Constant*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::Constant::STRUCT, MI::Project*);

  for (Constant* c : args)
    APPEND_STRINGS(c, project->library_paths);
  return 1;
}

int MI::function::exec_include(void* ptr, std::vector<Constant*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::Constant::STRUCT, MI::Project*);

  for (Constant* c : args)
    APPEND_STRINGS(c, project->includes);
  return 1;
}

int MI::function::exec_include_path(void* ptr, std::vector<Constant*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::Constant::STRUCT, MI::Project*);

  for (Constant* c : args)
    APPEND_STRINGS(c, project->include_paths);
  return 1;
}

int MI::function::exec_define(void* ptr, std::vector<Constant*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::Constant::STRUCT, MI::Project*);

  for (Constant* c : args)
    APPEND_STRINGS(c, project->definitions);
  return 1;
}

int MI::function::exec_source(void* ptr, std::vector<Constant*> &args, char* &info)
{
#define PROJECT_READ_SCRIPT() { \
  if (!file.exists()) \
  { \
    info = new char[32 + PATH_MAX]; \
    sprintf(info, "file not found '%s'", file.getPath().c_str()); \
    return 0; \
  }else if (!readScript(&file, (MI::Storage*) ptr)) \
    return 0; \
}
  for (Constant* c : args)
  {
    if (c->type == Constant::LIST)
    {
      for (Constant* c2 : *c->value.l)
      {
	me::File file(*c2->value.s);
	PROJECT_READ_SCRIPT();
      }
    }else
    {
      me::File file(*c->value.s);
      PROJECT_READ_SCRIPT();
    }
  }

  return 1;
}
