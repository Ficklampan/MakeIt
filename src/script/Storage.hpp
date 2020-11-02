#ifndef MI_STORAGE_HPP
  #define MI_STORAGE_HPP

#include "Variable.hpp"
#include "Function.hpp"

#include <map>
#include <string>

namespace MI {

  struct Storage {

    std::map<std::string, Variable*> variables;
    std::map<std::string, Function*> functions;

  };

}

#endif
