#ifndef MAKE_FILE_H
  #define MAKE_FILE_H

#include "MakeIt.h"

int MI_makefile(char* filepath, struct array* flags, struct array* sources, struct array* headers, struct makeit_project* project);

#endif
