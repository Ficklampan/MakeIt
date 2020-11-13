#ifndef MI_COMMON_HPP
  #define MI_COMMON_HPP

#include "Token.hpp"

namespace MI {

  void printError(Token::Location* loc, const char* format, ...);
  void printWarning(Token::Location* loc, const char* format, ...);

}

#endif
