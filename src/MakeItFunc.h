#ifndef MAKE_IT_FUNC_H
  #define MAKE_IT_FUNC_H

#include "MakeIt.h"

void usage_function(const char* func);
int makeit_process_functions(makeit_project* project, const char* func, const array* elements, const char* directory);

#endif
