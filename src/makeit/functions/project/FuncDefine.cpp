#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

#include "Util.hpp"

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
	  {
	    std::string* str = project->definitions[*name->as_string()] = new std::string("\\\"" + *value->as_string() + "\\\"");
	    makeit::string_replace(*str, " ", "\\ ");
	  }

	}else if (variable->type == Variable::STRING)
	{
	  project->definitions[*variable->as_string()] = nullptr;
	}

	return 1;

      });
  }
  return 1;
}
