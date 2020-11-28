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
	  StructArg::Arg("type", true, new Argument(Variable::STRING)),
	  StructArg::Arg("name", true, new Argument(Variable::STRING)),
	  StructArg::Arg("target", true, new Argument(Variable::STRING)),
	  StructArg::Arg("args", false, new Argument(Variable::STRING))
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

    std::string &type_str = *st["type"]->as_string();
    std::string &name = *st["name"]->as_string();
    std::string &target = *st["target"]->as_string();
    std::string* args = st.find("args") == st.end() ? nullptr : st["args"]->as_string();

    ExternConfig::Type type;

    if (type_str.compare("makeit") == 0)
      type = ExternConfig::MAKEIT;
    else if (type_str.compare("cmake") == 0)
      type = ExternConfig::CMAKE;
    else if (type_str.compare("premake") == 0)
      type = ExternConfig::PREMAKE;
    else if (type_str.compare("gnumake") == 0)
      type = ExternConfig::GNUMAKE;
    else if (type_str.compare("shell") == 0)
      type = ExternConfig::SHELL;
    else
      throw Exception(var->location, EUNKNOWN_CONFIG_TYPE, { type_str.c_str() });

    project->config.extern_configs.emplace_back(type, name, target, args == nullptr ? "" : *args);
  }
  return 1;
}
