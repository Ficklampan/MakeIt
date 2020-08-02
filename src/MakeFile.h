#ifndef MAKE_FILE_H
  #define MAKE_FILE_H

#include "utils/Map.h"

int make_makefile(char* name, char* directory, char* filepath, char* flags, char* sources, char* headers, char* libs, array* include_paths, array* lib_paths, array* definitions, char* build_path, char* src_ext, char** info);

#endif
