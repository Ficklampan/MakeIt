#include "../Functions.hpp"

MI::Function* MI::function::getFunction(void* ptr, const std::string &name)
{
  if (!name.compare("print")) return new Function(ptr, print);
  if (!name.compare("system")) return new Function(ptr, system);
  if (!name.compare("search")) return new Function(ptr, search);

  if (!name.compare("project")) return new Function(ptr, project);
  if (!name.compare("source")) return new Function(ptr, source);

  if (!name.compare("makefile")) return new Function(ptr, makefile);
  return nullptr;
}

void MI::function::usage(const std::string &func)
{
  // TODO
}
