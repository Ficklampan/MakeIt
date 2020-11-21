#include "Functions.hpp"

#include "Common.hpp"

#include "../Project.hpp"

#include "../script/Script.hpp"

#include <climits>

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

makeit::Function* makeit::function::make_library()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 1) | (Variable::LIST << 5)
      }, exec_library);
}
int makeit::function::exec_library(void* ptr, std::vector<Variable*> &args, char* &info)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE(project, Variable::POINTER);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    APPEND_STRINGS(v, project->libraries, storage);

  return 1;
}

makeit::Function* makeit::function::make_library_path()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 1) | (Variable::LIST << 5)
      }, exec_library_path);
}
int makeit::function::exec_library_path(void* ptr, std::vector<Variable*> &args, char* &info)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE(project, Variable::POINTER);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    APPEND_STRINGS(v, project->library_paths, storage);
  return 1;
}

makeit::Function* makeit::function::make_include()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 1) | (Variable::LIST << 5)
      }, exec_include);
}
int makeit::function::exec_include(void* ptr, std::vector<Variable*> &args, char* &info)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE(project, makeit::Variable::POINTER);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    APPEND_STRINGS(v, project->includes, storage);
  return 1;
}

makeit::Function* makeit::function::make_include_path()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 1) | (Variable::LIST << 5)
      }, exec_include_path);
}
int makeit::function::exec_include_path(void* ptr, std::vector<Variable*> &args, char* &info)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE(project, makeit::Variable::POINTER);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
    APPEND_STRINGS(v, project->include_paths, storage);
  return 1;
}

static std::map<std::string, makeit::Variable::Type> struct_define;
makeit::Function* makeit::function::make_define()
{
  struct_define["name"] = Variable::STRING;
  struct_define["value"] = Variable::VOID;

  return new Function(1, 
      new uint16_t[1]{
      1 | (Variable::STRING << 1) | (Variable::LIST << 5) | (Variable::STRUCT << 9)
    }, exec_define);
}
int makeit::function::exec_define(void* ptr, std::vector<Variable*> &args, char* &info)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE(project, makeit::Variable::POINTER);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* v : args)
  {
    LOOP_VARIABLES(v, [project, &info](Variable* variable) -> int {

	if (variable->type == Variable::STRUCT)
	{
	  if (!util::variable_check_struct(variable, struct_define, info))
	    return 0;

	  Variable::v_struct &st = *variable->as_struct();

	  Variable* name = st["name"];
	  Variable* value = st["value"];

	  if (value->type == Variable::STRING)
	    project->definitions[*name->as_string()] = new std::string('"' + *value->as_string() + '"');

	}else if (variable->type == Variable::STRING)
	{
	  project->definitions[*variable->as_string()] = nullptr;
	}

	return 1;

      });
  }
  return 1;
}

makeit::Function* makeit::function::make_source()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 1) | (Variable::LIST << 5)
      }, exec_source);
}
int makeit::function::exec_source(void* ptr, std::vector<Variable*> &args, char* &info)
{
#define PROJECT_READ_SCRIPT() { \
  if (!file.exists()) \
  { \
    info = new char[32 + PATH_MAX]; \
    sprintf(info, "file not found '%s'", file.getPath().c_str()); \
    return 0; \
  }else if (!readScript(&file, (makeit::Storage*) ptr)) \
    return 0; \
}

  for (Variable* v : args)
  {
    if (v->type == Variable::LIST)
    {
      for (Variable* v2 : *v->as_list())
      {
	if (v2->type != Variable::STRING)
	  continue;

	me::File file(*v2->as_string());
	PROJECT_READ_SCRIPT();
      }
    }else
    {
      me::File file(*v->as_string());
      PROJECT_READ_SCRIPT();
    }
  }

  return 1;
}
