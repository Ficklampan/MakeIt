#ifndef FILE_UTILS_H
  #define FILE_UTILS_H

#include "Type.h"

#include "Arrays.h"

#include <dirent.h>

/* returns file data */
uint8_t* fsurd(const char* filepath, uint32_t* length);

int fsuwr(const char* filepath, uint8_t* data, uint32_t length);
int fsumkd(const char* directory);

bool fsuexist(const char* filepath);

/* returns number of files found */
uint32_t fsufind(const char* directory, const char* pattern, array* arr, bool sub_dirs);

#endif
