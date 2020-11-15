#ifndef MAKE_IT_FUNC_HPP
  #define MAKE_IT_FUNC_HPP

#include "../script/Function.hpp"

#include <map>

namespace MI { namespace function {

  int exec_print(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_system(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_search(void* ptr, std::vector<Constant*> &args, char* &info);

  int exec_project(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_library(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_library_path(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_include(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_include_path(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_define(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_source(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_makefile(void* ptr, std::vector<Constant*> &args, char* &info);
  int exec_ycm(void* ptr, std::vector<Constant*> &args, char* &info);

  void usage(const std::string &func);

  MI::Function* getPrint();
  MI::Function* getSystem();
  MI::Function* getSearch();

  MI::Function* getProject();
  MI::Function* getLibrary();
  MI::Function* getLibraryPath();
  MI::Function* getInclude();
  MI::Function* getIncludePath();
  MI::Function* getDefine();
  MI::Function* getSource();
  MI::Function* getMakefile();
  MI::Function* getYCM();
  
} }

#endif
