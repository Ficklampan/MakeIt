#include "Functions.hpp"

#include "Common.hpp"

#include <lme/file.hpp>

#include <climits>
#include <fnmatch.h>

#include <functional>

int MI::function::exec_search(void* ptr, std::vector<VariableRef*> &args, char* &info)
{
  std::vector<std::string*> locations;
  std::vector<std::string*> patterns;

  APPEND_STRINGS(args.at(0), locations);
  APPEND_STRINGS(args.at(1), patterns);

  bool sub_dirs = VARIABLE_BOOLEAN(args.at(2))->value;

  MI::VariableRef* output = args.at(3);

  for (std::string* loc : locations)
  {
    me::File file(*loc);
    if (!file.exists() || !file.isDirectory())
    {
      info = new char[PATH_MAX + 32];
      sprintf(info, "directory not found '%s'", file.getPath().c_str());
      return 0;
    }

    std::function<int(me::File&)> callback = [output, patterns](me::File& file) -> int {
      for (std::string* pattern : patterns)
      {
	if (fnmatch(pattern->c_str(), file.getPath().c_str(), 0) == 0)
	  (*output) += new MI::VarString(file.getPath());
      }
      return 1;
    };

    size_t files = file.listFiles(sub_dirs, callback);
  }

  return 1;
}
