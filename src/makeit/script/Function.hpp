#ifndef MI_FUNCTION_HPP
  #define MI_FUNCTION_HPP

#include "Variable.hpp"

#include "../Text.hpp"

namespace makeit {

  class Function {

  public:

    const std::vector<Argument*> args;
    int (*exec) (void*, std::vector<Variable*>&);

    Function(const std::vector<Argument*> &args, int (*exec) (void*, std::vector<Variable*>&))
      : args(args)
    {
      this->exec = exec;
    }

  };

}

#endif
