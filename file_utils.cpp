#include "file_utils.h"
#include <stdio.h>

unsigned char* file_utils::read_file(const std::string &filepath)
{
  FILE* file = fopen(filepath.c_str(), "rb");
  fseek(file, 0, SEEK_END);
  unsigned int file_size = ftell(file);
  unsigned char* buffer = new unsigned char[file_size];
  rewind(file);
  fread((char*) buffer, file_size, 1, file);
  fclose(file);
  return buffer;
}

void file_utils::write_file(const std::string &filepath, unsigned char* data, unsigned int len)
{
  FILE* file = fopen(filepath.c_str(), "wb");
  fwrite(data, 1, len, file);
  fclose(file);
}
