#ifndef MAKE_IT_FUNC_H
  #define MAKE_IT_FUNC_H

#include "MakeIt.h"

#include "script/mtype.h"

void MIFUNC_usage(const char* func);

mvar* MIFUNC_makefile(mscript* script, mscope* scope, array* args);
enum mvar_t* MIFUNCARGS_makefile();

mvar* MIFUNC_project(mscript* script, mscope* scope, array* args);
enum mvar_t* MIFUNCARGS_project();

#endif
