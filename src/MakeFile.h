#ifndef MAKE_FILE_H
  #define MAKE_FILE_H

#include "utils/Map.h"

int make_makefile(const char* name, const char* directory, const char* filepath, const char* flags, array* sources, array* headers, array* libs, array* include_paths, array* lib_paths, array* definitions, const char* build_path, char** info);

#endif
