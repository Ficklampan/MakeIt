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

  for (Variable* var : *args.at(0)->as_list())
  {
    Variable::v_struct &st = *var->as_struct();

    Variable* name = st["name"];
    Variable* value = st.find("value") == st.end() ? nullptr : st["value"];

    if (value == nullptr)
      project->config.definitions.push_back(*name->as_string());
    else if (value->type == Variable::STRING)
    {
      std::string value_str = std::string("\\\"" + *value->as_string() + "\\\"");
      makeit::string_replace(value_str, " ", "\\ ");
      project->config.definitions.push_back(*name->as_string() + "=" + value_str);
    }
  }
  return 1;
}
