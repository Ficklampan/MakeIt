#ifndef MI_COMMON_HPP
  #define MI_COMMON_HPP

#include "Token.hpp"

namespace MI {

  void printError(char* str, Token::Location* loc);
  void printWarning(char* str, Token::Location* loc);

}

#endif
