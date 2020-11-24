#ifndef MI_FUNCTION_HPP
  #define MI_FUNCTION_HPP

#include "Variable.hpp"

#include "../Text.hpp"

namespace makeit {

  class Function {

  public:

    /*
     * [0]: endless
     * [1]: optional
     * [2 - 5]: type1
     * [6 - 9]: type2
     * [10 - 13]: type3
     */
    uint8_t argc;
    uint16_t* argv;

    int (*exec) (void*, std::vector<Variable*>&, char*&);

    Function(uint8_t argc, uint16_t* argv, int (*exec) (void*, std::vector<Variable*>&, char*&))
    {
      this->argc = argc;
      this->argv = argv;
      this->exec = exec;
    }

  };

}

#endif
