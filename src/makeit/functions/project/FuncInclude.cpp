#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"

makeit::Function* makeit::function::make_include()
{
  return new Function({
      new ListArg(new StructArg({
	    StructArg::Arg("pkg", false, new StructArg({
		  StructArg::Arg("flags", true, new ListArg(new Argument(Variable::STRING))),
		  StructArg::Arg("name", true, new Argument(Variable::STRING))
		  })),
	    StructArg::Arg("include", false, new Argument(Variable::STRING))
	    }))
      }, exec_include);
}

int makeit::function::exec_include(void* ptr, std::vector<Variable*> &args)
{
  makeit::Storage* storage = (makeit::Storage*) ptr;

  REQUIRE_VARIABLE("project", makeit::Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  Variable::v_list &list = *args.at(0)->as_list();

  for (const Variable* var : list)
  {
    Variable::v_struct &st = *var->as_struct();
    bool has_pkg = st.find("pkg") != st.end();
    bool has_inc = st.find("include") != st.end();

    if (has_pkg)
    {
      Variable::v_struct &pkg_st = *st["pkg"]->as_struct();

      std::vector<std::string> flags;
      GET_STRINGS(pkg_st["flags"], flags);
      const std::string &name = *pkg_st["name"]->as_string();

      project->config.cpackage_configs.emplace_back(flags, name);
    }

    if (has_inc)
    {
      project->config.includes.push_back(*st["include"]->as_string());
    }
  }

  return 1;
}
