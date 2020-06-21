#ifndef STRING_UTILS_HPP
  #define STRING_UTILS_HPP

#include <string>
#include <vector>

namespace string_utils {

  bool starts_with(const std::string &string, const std::string &with);
  void replace(std::string& str, const std::string& from, const std::string& to);
  std::vector<std::string> split(const std::string &str, char delimeter);
  std::string file_extension(const std::string &string, const std::string &new_extension);
  std::string directory(const std::string &filepath);
  std::string fix_path(const std::string &filepath, bool folder);
  unsigned int index(std::string &string, char c);

};

#endif