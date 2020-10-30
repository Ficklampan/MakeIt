#ifndef MAKE_IT_FUNC_HPP
  #define MAKE_IT_FUNC_HPP

#include "script/Script.hpp"

namespace mi { namespace function {
  
  void usage(const std::string &func);

  int print(std::vector<mi::Variable*> &variables);
  int system(std::vector<mi::Variable*> &variables);
  int search(std::vector<mi::Variable*> &variables);
  
  int project(std::vector<mi::Variable*> &variables);
  int script(std::vector<mi::Variable*> &variables);
  
  int define(std::vector<mi::Variable*> &variables);
  int library(std::vector<mi::Variable*> &variables);
  int include(std::vector<mi::Variable*> &variables);
  int library_dir(std::vector<mi::Variable*> &variables);
  int include_dir(std::vector<mi::Variable*> &variables);
  
  int make_config(std::vector<mi::Variable*> &variables);
  int ycm_config(std::vector<mi::Variable*> &variables);

}}

#endif
