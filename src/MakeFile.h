#ifndef MAKE_FILE_H
  #define MAKE_FILE_H

#include "utils/Map.h"

int make_makefile(char* name, char* version, char* directory, char* filepath, char* flags,
  array* sources,
  array* headers,
  array* libs,
  array* incs,
  array* include_paths,
  array* lib_paths,
  array* definitions,
  char* lang,
  char* build_path, char** info);

#endif
