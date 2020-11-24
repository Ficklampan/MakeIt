#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

#include "Util.hpp"

static std::map<std::string, uint16_t> struct_define;

makeit::Function* makeit::function::make_define()
{
  struct_define["name"] = 0 | (Variable::STRING << 2);
  struct_define["value"] = 0 | (Variable::VOID << 2);

  return new Function(1, 
      new uint16_t[1]{
      1 | (Variable::STRING << 2) | (Variable::LIST << 6) | (Variable::STRUCT << 10)
    }, exec_define);
}

int makeit::function::exec_define(void* ptr, std::vector<Variable*> &args, char* &info)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE("project", makeit::Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (uint32_t i = 0; i < args.size(); i++)
  {
    Variable* v = args.at(i);
    LOOP_VARIABLES(v, [i, project, &info](Variable* variable) -> int {

	if (variable->type == Variable::STRUCT)
	{
	  Variable::v_struct &st = *variable->as_struct();

	  try {
	    ERROR_CHECK_STRUCT(st, struct_define);
	  }catch (const Exception<int> &e)
	  {
	    throw Exception(i, e.get_err(), e.get_args());
	  }

	  Variable* name = st["name"];
	  Variable* value = st["value"];

	  if (value->type == Variable::STRING)
	  {
	    std::string* str = project->config.definitions[*name->as_string()] = new std::string("\\\"" + *value->as_string() + "\\\"");
	    makeit::string_replace(*str, " ", "\\ ");
	  }

	}else if (variable->type == Variable::STRING)
	{
	  project->config.definitions[*variable->as_string()] = nullptr;
	}

	return 1;

      });
  }
  return 1;
}
