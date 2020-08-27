#ifndef MIVM_H
  #define MIVM_H

#include "mtype.h"

int MIVM_compile(array* tokens, mscript* script);
int MIVM_procscope(iterator* iter, mscript* script, mscope* scope);

int MIVM_proctokens(iterator* iter, mscript* script, mscope* scope, mvar** var);
int MIVM_procfunc(mfunc_call* func_call, mscript* script, mscope* scope, mvar** var);
mvar* MIVM_procoprs(enum mopr_t opr_type, mvar* v1, mvar* v2);

#endif
