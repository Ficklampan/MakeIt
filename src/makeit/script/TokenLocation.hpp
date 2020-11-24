#ifndef MAKEIT_TOKEN_LOCATION_HPP
  #define MAKEIT_TOKEN_LOCATION_HPP

#include <lme/file.hpp>

namespace makeit {

  /* Token Location in file */
  struct TokenLocation {
    me::File* file;
    std::string* source;
    uint32_t pos, line, column, first;
  };

}

#endif
