#ifndef STRING_UTILS_H
  #define STRING_UTILS_H

#include <string>
#include <vector>

namespace string_utils {

  void replace(std::string& str, const std::string& from, const std::string& to);
  std::vector<std::string> split(std::string &str, char delimeter);
  std::string file_extension(const std::string &string, const std::string &new_extension);
  std::string directory(const std::string &filepath);
  unsigned int index(std::string &string, char c);

};

#endif
