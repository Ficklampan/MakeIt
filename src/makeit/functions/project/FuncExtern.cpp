#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"
#include "../../script/Script.hpp"

#include <lme/file.hpp>

#include <climits>

makeit::Function* makeit::function::make_extern()
{
  return new Function({
      new StructArg({
	  StructArg::Arg("name", false, new Argument(Variable::STRING)),
	  StructArg::Arg("target", true, new Argument(Variable::STRING)),
	  StructArg::Arg("prerequisites", true, new Argument(Variable::STRING)),
	  StructArg::Arg("command", true, new ListArg(new Argument(Variable::STRING))),
	  StructArg::Arg("phony", false, new Argument(Variable::INTEGER)),
	  }, Argument::ENDLESS)
      }, exec_extern);
}

int makeit::function::exec_extern(void* ptr, std::vector<Variable*> &args)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE("project", makeit::Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  for (Variable* var : args)
  {
    Variable::v_struct &st = *var->as_struct();

    const std::string &name = st.find("name") == st.end() ? "" : *st["target"]->as_string();
    const std::string &target = *st["target"]->as_string();
    const std::string &prerequisites = *st["prerequisites"]->as_string();
    const bool phony = st.find("phony") == st.end() ? false : *st["phony"]->as_integer();

    std::vector<std::string> commands;
    GET_STRINGS(st["command"], commands);

    project->config.extern_configs.emplace_back(name, target, prerequisites, commands, phony);
  }
  return 1;
}
