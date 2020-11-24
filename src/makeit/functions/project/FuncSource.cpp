#include "../Functions.hpp"

#include "../FuncUtils.hpp"
#include "../../Project.hpp"
#include "../../script/Script.hpp"

#include <lme/file.hpp>

#include <climits>

makeit::Function* makeit::function::make_source()
{
  return new Function(1,
      new uint16_t[1]{
      1 | (Variable::STRING << 2) | (Variable::LIST << 6)
      }, exec_source);
}

int makeit::function::exec_source(void* ptr, std::vector<Variable*> &args, char* &info)
{
#define PROJECT_READ_SCRIPT() { \
  if (!file.exists()) \
  { \
    info = new char[32 + PATH_MAX]; \
    sprintf(info, "file not found '%s'", file.getPath().c_str()); \
    return 0; \
  }else if (!read_script(&file, (makeit::Storage*) ptr)) \
    return 0; \
}

  for (Variable* v : args)
  {
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
