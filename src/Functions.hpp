#ifndef MAKE_IT_FUNC_HPP
  #define MAKE_IT_FUNC_HPP

#include "script/Function.hpp"

namespace MI { namespace function {

  MI::Function* getFunction(void* ptr, const std::string &name);
  
  void usage(const std::string &func);

  int print(void* ptr, std::vector<Constant*> &args, char* &info);
  int system(void* ptr, std::vector<Constant*> &args, char* &info);
  int search(void* ptr, std::vector<Constant*> &args, char* &info);

  int project(void* ptr, std::vector<Constant*> &args, char* &info);
  int source(void* ptr, std::vector<Constant*> &args, char* &info);
  int makefile(void* ptr, std::vector<Constant*> &args, char* &info);
  int ycm(void* ptr, std::vector<Constant*> &args, char* &info);
  
} }

#endif
