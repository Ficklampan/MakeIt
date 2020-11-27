#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"
#include "../../script/Script.hpp"

#include <lme/file.hpp>

#include <climits>

makeit::Function* makeit::function::make_source()
{
  return new Function({
      new Argument(Variable::STRING, Argument::ENDLESS)
      }, exec_source);
}

int makeit::function::exec_source(void* ptr, std::vector<Variable*> &args)
{
#define PROJECT_READ_SCRIPT() { \
  if (!file.exists()) \
  { \
    throw Exception((int) i, EFILE_NOT_FOUND, { file.getPath().c_str() }); \
  }else if (!read_script(&file, (makeit::Storage*) ptr)) \
    throw Exception((int) i, EFAILED_TO_READ_FILE, { file.getPath().c_str() }); \
}

  for (uint32_t i = 0; i < args.size(); i++)
  {
    Variable* v = args.at(i);
    if (v->type == Variable::LIST)
    {
      for (Variable* v2 : *v->as_list())
      {
	if (v2->type != Variable::STRING)
	  continue;

	me::File file(*v2->as_string());
	PROJECT_READ_SCRIPT();
      }
    }else
    {
      me::File file(*v->as_string());
      PROJECT_READ_SCRIPT();
    }
  }

  return 1;
}
