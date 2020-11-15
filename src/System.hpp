#ifndef MI_SYSTEM_HPP
  #define MI_SYSTEM_HPP

#include <lme/file.hpp>

namespace MI {

  int readFile(const me::File &file, void* &data, uint32_t &size);
  int writeFile(const me::File &file, void* data, uint32_t size);
  int system(const char* cmd);

}

#endif
