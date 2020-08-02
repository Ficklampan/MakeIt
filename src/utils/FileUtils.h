#ifndef FILE_UTILS_H
  #define FILE_UTILS_H

#include "Type.h"

#include "Map.h"

#include <dirent.h>

uint8_t* file_utils_read(const char* filepath, uint32_t* length);
void file_utils_write(const char* filepath, uint8_t* data, uint32_t length);
array* file_utils_find(const char* directory, const char* pattern, array* arr, bool sub_dirs);

#endif
