#ifndef MI_FUNC_H
  #define MI_FUNC_H

#include "../utils/Type.h"
#include "../utils/Arrays.h"

#include "mtype.h"

#include <stdlib.h>
#include <stdio.h>

static inline mfunc* fnew(uint32_t argc, enum mvar_t* args, mvar* (*exec) (array*))
{
  mfunc* func = (mfunc*) calloc(sizeof(mfunc), 1);
  func->argc = argc;
  func->args = args;
  func->exec = exec;
  return func;
}

#endif
