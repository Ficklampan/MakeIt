#ifndef MAKEIT_UTIL_HPP
  #define MAKEIT_UTIL_HPP

#include <string>

namespace makeit {

  void string_replace(std::string &str, const std::string &target, const std::string &with);

  inline bool get_bit_flag(uint64_t flags, uint64_t flag)
  {
    return (flags & flag) > 0;
  }

}

#endif
