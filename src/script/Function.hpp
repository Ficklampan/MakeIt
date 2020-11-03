#ifndef MI_FUNCTION_HPP
  #define MI_FUNCTION_HPP

#include "Variable.hpp"

namespace MI {

  struct Function {

    typedef int (func_exec) (void*, std::vector<Constant*>&, char* &info);

  private:

    void* ptr;
    func_exec* exec;

  public:

    Function(void* ptr, func_exec* exec)
    {
      this->ptr = ptr;
      this->exec = exec;
    }

    int execute(std::vector<Constant*> &args, char* &info)
    {
      return exec(ptr, args, info);
    }

  };

}

#endif
