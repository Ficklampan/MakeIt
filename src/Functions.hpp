#ifndef MAKE_IT_FUNC_HPP
  #define MAKE_IT_FUNC_HPP

#include "script/Function.hpp"

#include <map>

namespace MI { namespace function {

  void init();
  MI::Function* getFunction(const std::string &name);
  std::map<std::string, MI::Function*> getAllFunctions();

  void usage(const MI::Function* func);

  struct Print : MI::Function { Print(); int execute(void* ptr, std::vector<Constant*> &args, char* &info) override; };
  struct System : MI::Function { System(); int execute(void* ptr, std::vector<Constant*> &args, char* &info) override; };
  struct Search : MI::Function { Search(); int execute(void* ptr, std::vector<Constant*> &args, char* &info) override; };

  struct Project : MI::Function { Project(); int execute(void* ptr, std::vector<Constant*> &args, char* &info) override; };
  struct Source : MI::Function { Source(); int execute(void* ptr, std::vector<Constant*> &args, char* &info) override; };
  struct Makefile : MI::Function { Makefile(); int execute(void* ptr, std::vector<Constant*> &args, char* &info) override; };
  struct YCM : MI::Function { YCM(); int execute(void* ptr, std::vector<Constant*> &args, char* &info) override; };
  
} }

#endif
