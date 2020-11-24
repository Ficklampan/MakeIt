#ifndef MAKEIT_FUNC_HPP
  #define MAKEIT_FUNC_HPP

#include "../script/Function.hpp"
#include "../script/Storage.hpp"

namespace makeit { namespace function {

  int exec_print(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_system(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_search(void* ptr, std::vector<Variable*> &args, char* &info);

  int exec_project(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_library(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_library_path(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_include(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_include_path(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_define(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_flags(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_source(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_files(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_makefile(void* ptr, std::vector<Variable*> &args, char* &info);
  int exec_ycm(void* ptr, std::vector<Variable*> &args, char* &info);

  void usage(const std::string &func);

  makeit::Function* make_print();
  makeit::Function* make_system();
  makeit::Function* make_search();

  makeit::Function* make_project();
  makeit::Function* make_library();
  makeit::Function* make_library_path();
  makeit::Function* make_include();
  makeit::Function* make_include_path();
  makeit::Function* make_define();
  makeit::Function* make_flags();
  makeit::Function* make_source();
  makeit::Function* make_files();
  makeit::Function* make_makefile();
  makeit::Function* make_YCM();
  
} }

#endif
