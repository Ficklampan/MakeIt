#ifndef TRIGGER_HPP
  #define TRIGGER_HPP

#include "errors.hpp"
#include "../makeit.hpp"

#include <string>
#include <vector>

namespace makeit {

  struct Trigger {
    /*
      0: fail
      1: success
      2: exploded
    */
    virtual int triggered(makeit::project* project, const std::string &prefix, const std::vector<std::string> &elements, unsigned int index) = 0;
  };

}

#endif
