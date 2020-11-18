#include "Functions.hpp"

#include "Common.hpp"

#include "../Project.hpp"

#include "../script/Script.hpp"

#include <climits>

int MI::function::exec_project(void* ptr, std::vector<VariableRef*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  MI::Project* project = new MI::Project;
  project->name = VARIABLE_STRING(args.at(0))->value;
  project->version = VARIABLE_STRING(args.at(1))->value;
  project->language = VARIABLE_STRING(args.at(2))->value;

  storage->variables["project"] = new MI::VarStruct(project);

  return 1;
}

int MI::function::exec_library(void* ptr, std::vector<VariableRef*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::VariableRef::STRUCT);

  MI::Project* project = (MI::Project*) VARIABLE_STRUCT(storage->variables["project"])->value;

  for (VariableRef* c : args)
    APPEND_STRINGS(c, project->libraries);

  return 1;
}

int MI::function::exec_library_path(void* ptr, std::vector<VariableRef*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::VariableRef::STRUCT);

  MI::Project* project = (MI::Project*) VARIABLE_STRUCT(storage->variables["project"])->value;

  for (VariableRef* c : args)
    APPEND_STRINGS(c, project->library_paths);
  return 1;
}

int MI::function::exec_include(void* ptr, std::vector<VariableRef*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::VariableRef::STRUCT);

  MI::Project* project = (MI::Project*) VARIABLE_STRUCT(storage->variables["project"])->value;

  for (VariableRef* c : args)
    APPEND_STRINGS(c, project->includes);
  return 1;
}

int MI::function::exec_include_path(void* ptr, std::vector<VariableRef*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::VariableRef::STRUCT);

  MI::Project* project = (MI::Project*) VARIABLE_STRUCT(storage->variables["project"])->value;

  for (VariableRef* c : args)
    APPEND_STRINGS(c, project->include_paths);
  return 1;
}

int MI::function::exec_define(void* ptr, std::vector<VariableRef*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;

  REQUIRE_VARIABLE(project, MI::VariableRef::STRUCT);

  MI::Project* project = (MI::Project*) VARIABLE_STRUCT(storage->variables["project"])->value;

  for (VariableRef* c : args)
    APPEND_STRINGS(c, project->definitions);
  return 1;
}

int MI::function::exec_source(void* ptr, std::vector<VariableRef*> &args, char* &info)
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

  for (VariableRef* v : args)
  {
    if (v->type == VariableRef::LIST)
    {
      for (VariableRef* v2 : VARIABLE_LIST(v)->value)
      {
	me::File file(VARIABLE_STRING(v2)->value);
	PROJECT_READ_SCRIPT();
      }
    }else
    {
      me::File file(VARIABLE_STRING(v)->value);
      PROJECT_READ_SCRIPT();
    }
  }

  return 1;
}
