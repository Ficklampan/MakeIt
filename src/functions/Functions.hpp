#ifndef MAKE_IT_FUNC_HPP
  #define MAKE_IT_FUNC_HPP

#include "../script/Function.hpp"

#include <map>

namespace MI { namespace function {

  int exec_print(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_system(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_search(void* ptr, std::vector<Constant*> &args, char* &info);

  int exec_project(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_source(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_makefile(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_ycm(void* ptr, std::vector<Constant*> &args, char* &info);

  void usage(const std::string &func);

  MI::Function* getPrint();
  MI::Function* getSystem();
  MI::Function* getSearch();

  MI::Function* getProject();
  MI::Function* getSource();
  MI::Function* getMakefile();
  MI::Function* getYCM();
  
} }

#endif
