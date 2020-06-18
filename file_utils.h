#ifndef FILE_UTILS_H
  #define FILE_UTILS_H

#include <string>
#include <vector>

namespace file_utils {

  unsigned char* read_file(const std::string &filepath);
  void write_file(const std::string &filepath, unsigned char* data, unsigned int len);
  void get_file_name(const std::string &filepath, std::string &name, std::string &ext);
  std::vector<std::string> find_files(const std::string &directory, const std::string &fileName, const std::string &fileExt);

};

#endif
