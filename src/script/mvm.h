#ifndef MVM_H
  #define MVM_H

#include "mtype.h"

mvar* MILEX_procfunc(mfunc_call* func_call, mscript* script, mscope* scope);

mvar* MILEX_procvals(iterator* iter, mscript* script, mscope* scope);
mvar* MILEX_procoprs(enum mtoken_t opr_type, mvar* v1, mvar* v2);

#endif
