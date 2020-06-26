#include "FileUtils.h"

#include <stdio.h>
#include <stdlib.h>

uint8_t* file_utils_read(const char* filepath, uint32_t* length)
{
  FILE* file = fopen(filepath, "rb");
  fseek(file, 0L, SEEK_END);
  *length = ftell(file);
  uint8_t* buffer = malloc(*length);
  rewind(file);
  fread(buffer, 1, *length, file);
  fclose(file);
  return buffer;
}
void file_utils_write(const char* filepath, uint8_t* data, uint32_t length)
{

}
