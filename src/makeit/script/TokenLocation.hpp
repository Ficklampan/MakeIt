#ifndef MAKEIT_TOKEN_LOCATION_HPP
  #define MAKEIT_TOKEN_LOCATION_HPP

#include <lme/file.hpp>

namespace makeit {

  /* Token Location in file */
  struct TokenLocation {

    const me::File* file;
    std::string* source;
    uint32_t begin = 0, begin_line = 1, pos = 0, size = 0;
    uint32_t line = 1, column = 1;

    TokenLocation(const me::File* file = nullptr, std::string* source = nullptr)
      : file(file)
    {
      this->source = source;
    }

  };

}

#endif
