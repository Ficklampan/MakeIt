#ifndef MI_COMMON_HPP
  #define MI_COMMON_HPP

#include "Token.hpp"

namespace makeit {

  void printError(TokenLocation &loc, const char* format, ...);
  void printWarning(TokenLocation &loc, const char* format, ...);

}

#endif
