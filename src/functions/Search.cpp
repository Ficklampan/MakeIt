#include "Functions.hpp"

#include "Common.hpp"

#include <lme/file.hpp>

#include <climits>
#include <fnmatch.h>

#include <functional>

makeit::Function* makeit::function::make_search()
{
  return new Function(4,
      new uint16_t[4]{
      0 | (Variable::STRING << 1) | (Variable::LIST << 5),
      0 | (Variable::STRING << 1) | (Variable::LIST << 5),
      0 | (Variable::INTEGER << 1),
      0 | (Variable::LIST << 1)
      }, exec_search);
}
int makeit::function::exec_search(void* ptr, std::vector<Variable*> &args, char* &info)
{
  Storage* storage = (Storage*) ptr;

  std::vector<std::string*> locations;
  std::vector<std::string*> patterns;

  APPEND_STRINGS(args.at(0), locations, storage);
  APPEND_STRINGS(args.at(1), patterns, storage);

  bool sub_dirs = args.at(2)->as_integer();

  makeit::Variable* output = args.at(3);
  output->as_list()->reserve(32);

  for (std::string* loc : locations)
  {
    me::File file(*loc);
    if (!file.exists() || !file.isDirectory())
    {
      info = new char[PATH_MAX + 32];
      sprintf(info, "directory not found '%s'", file.getPath().c_str());
      return 0;
    }

    std::function<int(me::File&)> callback = [storage, output, patterns](me::File& file) -> int {
      for (std::string* pattern : patterns)
      {
	const std::string &file_path = file.getPath();
	if (fnmatch(pattern->c_str(), file_path.c_str(), 0) == 0)
	  (*output) += new Variable(Variable::STRING, new std::string(file_path));
      }
      return 1;
    };

    size_t files = file.listFiles(sub_dirs, callback);
  }

  return 1;
}
