#ifndef MI_STORAGE_HPP
  #define MI_STORAGE_HPP

#include "Variable.hpp"
#include "Function.hpp"

#include <map>
#include <string>

namespace MI {

  /* Storage for all the variables and functions */
  struct Storage {

    std::map<std::string, Variable*> variables;
    std::map<std::string, Function*> functions;

  };

}

#endif
