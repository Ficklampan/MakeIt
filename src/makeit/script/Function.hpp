#ifndef MI_FUNCTION_HPP
  #define MI_FUNCTION_HPP

#include "Variable.hpp"

namespace makeit {

  struct Function {

    /*
     * [1]: endless
     * [1 - 4]: type1
     * [5 - 8]: type2
     * [9 - 12]: type3
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
