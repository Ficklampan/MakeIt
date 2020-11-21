#ifndef MAKEIT_SCRIPT_HPP
  #define MAKEIT_SCRIPT_HPP

#include "Storage.hpp"
#include "Token.hpp"

namespace makeit {

  int readScript(me::File* file, Storage* storage);

  void printError(TokenLocation &loc, const char* format, ...);
  void printWarning(TokenLocation &loc, const char* format, ...);



}

#endif
