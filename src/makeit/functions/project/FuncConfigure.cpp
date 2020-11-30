#include "../Functions.hpp"
#include "../FuncUtils.hpp"

#include "../../Project.hpp"

makeit::Function* makeit::function::make_configure()
{
  return new Function({
      new StructArg({
	  StructArg::Arg("optimization", true, new Argument(Variable::LITERIAL)),
	  StructArg::Arg("std", false, new Argument(Variable::LITERIAL)),
	  StructArg::Arg("lang", false, new Argument(Variable::LITERIAL)),
	  StructArg::Arg("cc", false, new Argument(Variable::LITERIAL)),
	  StructArg::Arg("kind", false, new Argument(Variable::LITERIAL)),
	  StructArg::Arg("build", false, new Argument(Variable::STRING)),
	  })
      }, exec_configure);
}

int makeit::function::exec_configure(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", makeit::Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  Variable::v_struct* st = args.at(0)->as_struct();

  for (auto &[key, value] : *st)
  {
    if (key.compare("optimization") == 0)
    {
      const std::string &value_str = *value->as_literial();

      if (value_str.compare("O0") == 0)
	project->config.optimization = BuildConfig::NONE;
      else if (value_str.compare("O1") == 0)
	project->config.optimization = BuildConfig::O1;
      else if (value_str.compare("O2") == 0)
	project->config.optimization = BuildConfig::O2;
      else if (value_str.compare("O3") == 0)
	project->config.optimization = BuildConfig::O3;
      else if (value_str.compare("Os") == 0)
	project->config.optimization = BuildConfig::Os;
      else if (value_str.compare("Ofast") == 0)
	project->config.optimization = BuildConfig::Ofast;
      else if (value_str.compare("Og") == 0)
	project->config.optimization = BuildConfig::Og;
      else if (value_str.compare("Oz") == 0)
	project->config.optimization = BuildConfig::Oz;

    }else if (key.compare("std") == 0)
    {
      const std::string &value_str = *value->as_literial();

      if (value_str.compare("STD98") == 0)
	project->config.cpp_version = BuildConfig::CPP_98;
      if (value_str.compare("STD11") == 0)
	project->config.cpp_version = BuildConfig::CPP_11;
      if (value_str.compare("STD14") == 0)
	project->config.cpp_version = BuildConfig::CPP_14;
      if (value_str.compare("STD17") == 0)
	project->config.cpp_version = BuildConfig::CPP_17;
      if (value_str.compare("STD20") == 0)
	project->config.cpp_version = BuildConfig::CPP_20;

    }else if (key.compare("lang") == 0)
    {
      const std::string &value_str = *value->as_literial();

      if (value_str.compare("C") == 0)
	project->config.lang = BuildConfig::C;
      else if (value_str.compare("CPP") == 0 || value_str.compare("CXX") == 0)
	project->config.lang = BuildConfig::CPP;

    }else if (key.compare("cc") == 0)
    {
      const std::string &value_str = *value->as_literial();

      if (value_str.compare("GNU") == 0)
	project->config.cc = BuildConfig::GNU;
      if (value_str.compare("LLVM") == 0)
	project->config.cc = BuildConfig::LLVM;

    }else if (key.compare("kind") == 0)
    {
      const std::string &value_str = *value->as_literial();

      if (value_str.compare("EXECUTABLE") == 0)
	project->config.kind = BuildConfig::EXECUTABLE;
      else if (value_str.compare("STATIC_LIBRARY") == 0)
	project->config.kind = BuildConfig::STATIC_LIBRARY;
      else if (value_str.compare("SHARED_LIBRARY") == 0)
	project->config.kind = BuildConfig::SHARED_LIBRARY;

    }else if (key.compare("build") == 0)
    {
      const std::string &value_str = *value->as_string();

      project->config.build = value_str;
    }

  }

  return 1;
}
