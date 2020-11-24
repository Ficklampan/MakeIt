#include "../Functions.hpp"

#include "../FuncUtils.hpp"

#include "../../configs/GNUMake.hpp"

#include "../../Project.hpp"
#include "../../System.hpp"

static std::map<std::string, uint16_t> struct_makefile;

makeit::Function* makeit::function::make_makefile()
{
  struct_makefile["comments"] = Variable::OPTIONAL | (Variable::INTEGER << 2);

  return new Function(2,
      new uint16_t[2]{
      0 | (Variable::STRING << 2),
      Variable::OPTIONAL | (Variable::STRUCT << 2)
      }, exec_makefile);
}

int makeit::function::exec_makefile(void* ptr, std::vector<Variable*> &args, char* &info)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  me::File file = *args.at(0)->as_string();
  GNUMake::Options options;

  if (args.size() > 0)
  {
    Variable::v_struct* st = args.at(1)->as_struct();

    try {
      ERROR_CHECK_STRUCT(*st, struct_makefile);
    }catch (const Exception<int> &e)
    {
      throw Exception(1, e.get_err(), e.get_args());
    }

    for (auto &[key, value] : *st)
    {
      if (key.compare("comments") == 0)
	options.comments = *value->as_integer() > 0;
    }
  }

  GNUMake makefile(project->name, &project->config, options);

  std::string makefile_data;
  makefile_data.reserve(4096);

  try {
    makefile.generate(makefile_data);
  }catch (const Exception<std::string> &e)
  {
    throw Exception(-1, EMAKEFILE, { e.get_location().c_str() });
  }

  makeit::writeFile(file, makefile_data.data(), makefile_data.size());
  return 1;
}
