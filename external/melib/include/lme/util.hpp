#ifndef MELIB_UTIL_HPP
  #define MELIB_UTIL_HPP

#include <string>
#include <vector>

namespace me { namespace string {

  size_t split(const std::string &str, char c, std::vector<std::string_view> &strs);

} }

#endif
