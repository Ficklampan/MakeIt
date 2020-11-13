#include "../Functions.hpp"

#include "../script/Storage.hpp"

#include "../configs/GNUMake.hpp"

MI::function::Makefile::Makefile()
{
  desc = "";
  args = ArgBuilder().type(MI::Constant::STRING)
    .type({MI::Constant::STRING, MI::Constant::LIST})
    .type({MI::Constant::STRING, MI::Constant::LIST})
    .build();
}

static inline int MAKECONFIG_LIST_STRINGS(std::vector<MI::Constant*> &constants, std::vector<std::string*> &strs, char* &info)
{
  for (uint32_t i = 0; i < constants.size(); i++)
  {
    MI::Constant* c = constants.at(i);
    if (c->type == MI::Constant::STRING)
      strs.push_back(c->value.s);
    else if (c->type == MI::Constant::LIST)
    {
      if (!MAKECONFIG_LIST_STRINGS(*c->value.l, strs, info))
	return 0;
    }else
    {
      info = new char[128];
      sprintf(info, "expected type 'string' or 'list' in array at [%u]", i);
      return 0;
    }
  }
  return 1;
}

/* TODO: make so you can put a string and not only a list */
int MI::function::Makefile::execute(void* ptr, std::vector<Constant*> &args, char* &info)
{
  MI::Storage* storage = (MI::Storage*) ptr;
  MI::Project* project = (MI::Project*) storage->variables["project"]->value.v;

  /* [Error] project not found */
  if (project == nullptr)
  {
    info = (char*) "project not found";
    return 0;
  }

  std::string filepath = *args.at(0)->value.s;

  std::vector<std::string*> sources;
  std::vector<std::string*> headers;

#ifndef MAKECONFIG_PROCESS_ARG
  #define MAKECONFIG_PROCESS_ARG(i, a) {\
    if (args.at(i)->type == MI::Constant::LIST) \
    { \
      if (!MAKECONFIG_LIST_STRINGS(*args.at(i)->value.l, sources, info)) \
        return 0; \
    }else if (args.at(i)->type == MI::Constant::STRING) \
      a.push_back(args.at(i)->value.s); \
  }
#endif

  MAKECONFIG_PROCESS_ARG(1, sources);
  MAKECONFIG_PROCESS_ARG(2, sources);

  GNUMake_config(filepath, sources, headers, project);
  return 1;
}
