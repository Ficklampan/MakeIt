#include "../Functions.hpp"

#include "../FuncUtils.hpp"

#include <lme/file.hpp>

#include <fnmatch.h>
#include <climits>

makeit::Function* makeit::function::make_search()
{
  return new Function(4,
      new uint16_t[4]{
      0 | (Variable::STRING << 2) | (Variable::LIST << 6),
      0 | (Variable::STRING << 2) | (Variable::LIST << 6),
      0 | (Variable::INTEGER << 2),
      0 | (Variable::LIST << 2)
      }, exec_search);
}
int makeit::function::exec_search(void* ptr, std::vector<Variable*> &args, char* &info)
{
  Storage* storage = (Storage*) ptr;

  std::vector<std::string*> locations;
  std::vector<std::string*> patterns;

  GET_STRINGS(args.at(0), locations);
  GET_STRINGS(args.at(1), patterns);

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
	  (*output) += new Variable(nullptr, Variable::STRING, new std::string(file_path));
      }
      return 1;
    };

    size_t files = file.listFiles(sub_dirs, callback);
  }

  return 1;
}
