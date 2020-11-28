#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

#include "Util.hpp"

makeit::Function* makeit::function::make_define()
{
  return new Function({
      new ListArg(new StructArg({
	    StructArg::Arg("name", true, new Argument(Variable::STRING)),
	    StructArg::Arg("value", false, new Argument(Variable::VOID))
	    }), Argument::ENDLESS)
      }, exec_define);
}

int makeit::function::exec_define(void* ptr, std::vector<Variable*> &args)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE("project", makeit::Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (uint32_t i = 0; i < args.size(); i++)
  {
    Variable* v = args.at(i);
    LOOP_VARIABLES(v, [i, project](Variable* variable) -> int {

	if (variable->type == Variable::STRUCT)
	{
	  Variable::v_struct &st = *variable->as_struct();

	  Variable* name = st["name"];
	  Variable* value = st["value"];

	  if (value->type == Variable::STRING)
	  {
	    std::string str = project->config.definitions[*name->as_string()] = std::string("\\\"" + *value->as_string() + "\\\"");
	    makeit::string_replace(str, " ", "\\ ");
	  }

	}else if (variable->type == Variable::STRING)
	{
	  project->config.definitions[*variable->as_string()] = "";
	}

	return 1;

      });
  }
  return 1;
}