#ifndef MI_FUNCTION_HPP
  #define MI_FUNCTION_HPP

#include "Variable.hpp"

namespace MI {

  struct Function {

    /*
     * [1]: endless
     * [1 - 3]: type1
     * [4 - 6]: type2
     * [7 - 9]: type3
     * [10 - 12]: type4
     * [13 - 15]: type5
     */
    uint8_t argc;
    uint16_t* argv;

    int (*exec) (void*, std::vector<Constant*>&, char*&);

  };

}

#endif
