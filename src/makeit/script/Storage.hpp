#ifndef MAKEIT_STORAGE_HPP
  #define MAKEIT_STORAGE_HPP

#include "Variable.hpp"
#include "Function.hpp"

#include <lme/file.hpp>

#include <map>
#include <string>

namespace makeit {

  /* Storage for all the variables and functions */
  struct Storage {

    std::map<std::string, std::string> definitions;
    std::map<std::string, Variable*> variables;
    std::map<std::string, Function*> functions;
    std::vector<me::File> scripts;

    ~Storage()
    {
    }

  };

}

#endif
