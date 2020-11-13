#include "../Functions.hpp"

static std::map<std::string, MI::Function*> functions;

void MI::function::init()
{
  functions["print"] = new function::Print;
  functions["system"] = new function::System;
  functions["search"] = new function::Search;

  functions["project"] = new function::Project;
  functions["makefile"] = new function::Makefile;
}

MI::Function* MI::function::getFunction(const std::string &name)
{
  return functions[name];
}

std::map<std::string, MI::Function*> MI::function::getAllFunctions()
{
  return functions;
}

void MI::function::usage(const MI::Function* func)
{
  // TODO
}
