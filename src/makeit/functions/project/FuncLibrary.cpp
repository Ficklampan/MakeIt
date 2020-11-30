#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_library()
{
  return new Function({
      new ListArg(new StructArg({
	    StructArg::Arg("pkg", false, new StructArg({
		  StructArg::Arg("static", true, new Argument(Variable::INTEGER)),
		  StructArg::Arg("flags", true, new ListArg(new Argument(Variable::STRING))),
		  StructArg::Arg("name", true, new Argument(Variable::STRING))
		  })),
	    StructArg::Arg("lib", false, new Argument(Variable::STRING))
	    }))
      }, exec_library);
}

int makeit::function::exec_library(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  Variable::v_list &list = *args.at(0)->as_list();

  for (const Variable* var : list)
  {
    Variable::v_struct &st = *var->as_struct();
    bool has_pkg = st.find("pkg") != st.end();
    bool has_lib = st.find("lib") != st.end();

    if (has_pkg)
    {
      Variable::v_struct &pkg_st = *st["pkg"]->as_struct();

      const bool static_ = *pkg_st["static"]->as_integer();
      std::vector<std::string> flags;
      GET_STRINGS(pkg_st["flags"], flags);
      const std::string &name = *pkg_st["name"]->as_string();

      project->config.lpackage_configs.emplace_back(static_, flags, name);
    }

    if (has_lib)
    {
      project->config.libraries.push_back(*st["lib"]->as_string());
    }
  }

  return 1;
}
