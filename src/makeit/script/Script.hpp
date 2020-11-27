#ifndef MAKEIT_SCRIPT_HPP
  #define MAKEIT_SCRIPT_HPP

#include "Storage.hpp"
#include "Token.hpp"

namespace makeit {

  int read_script(me::File* file, Storage* storage);

  void print_error(const TokenLocation* loc, const char* format, ...);
  void print_warning(const TokenLocation* loc, const char* format, ...);



}

#endif
