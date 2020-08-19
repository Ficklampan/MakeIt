#ifndef MAKE_IT_FUNC_H
  #define MAKE_IT_FUNC_H

#include "MakeIt.h"

void MIFUNC_usage(const char* func);
int MIFUNC_proc(makeit_project* project, const char* func, const array* elements, const char* directory);

#endif
