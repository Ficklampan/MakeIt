#include "Functions.hpp"

#include <lme/file.hpp>

#include <climits>
#include <cstring>
#include <fnmatch.h>

static inline void SEARCH_APPEND_STRINGS(MI::Constant* constant, std::vector<std::string*> &strs)
{
  if (constant->type == MI::Constant::STRING)
    strs.push_back(constant->value.s);
  else if (constant->type == MI::Constant::LIST)
  {
    for (MI::Constant* c : *constant->value.l)
      SEARCH_APPEND_STRINGS(c, strs);
  }
}

int MI::function::exec_search(void* ptr, std::vector<Constant*> &args, char* &info)
{
  std::vector<std::string*> locations;
  std::vector<std::string*> patterns;

  SEARCH_APPEND_STRINGS(args.at(0), locations);
  SEARCH_APPEND_STRINGS(args.at(1), patterns);

  bool sub_dirs = *args.at(2)->value.b;

  MI::Variable* output = (MI::Variable*) args.at(3);

  for (std::string* loc : locations)
  {
    me::File file(*loc);
    if (!file.exists() || !file.isDirectory())
    {
      info = new char[PATH_MAX + 32];
      sprintf(info, "directory not found '%s'", file.getPath().c_str());
      return 0;
    }

    std::vector<me::File*> files;
    file.listFiles(files, sub_dirs);

    for (me::File* file : files)
    {
      for (std::string* pattern : patterns)
      {
	if (fnmatch(pattern->c_str(), file->getPath().c_str(), 0) == 0)
	{
	  std::string path = file->getPath();
	  char* copy = new char[path.size()];
	  memcpy(copy, path.c_str(), path.size());
	  (*output) += *new MI::Constant(MI::Constant::STRING, new std::string(copy, path.size()));
	}
      }
    }

    /* free files */
    for (me::File* file : files)
      delete file;

  }

  return 1;
}
