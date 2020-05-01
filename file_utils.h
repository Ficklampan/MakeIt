#ifndef FILE_UTILS_H
  #define FILE_UTILS_H

#include <string>

namespace file_utils {

  unsigned char* read_file(const std::string &filepath);
  void write_file(const std::string &filepath, unsigned char* data, unsigned int len);

};

#endif
