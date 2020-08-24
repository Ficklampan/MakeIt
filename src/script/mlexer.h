#ifndef MAKE_IT_LEXER_H
  #define MAKE_IT_LEXER_H

#include "mtype.h"
#include "mtoken.h"
#include "mfunctions.h"

#include "../utils/Type.h"
#include "../utils/Arrays.h"
#include "../utils/String.h"

#include <stdio.h>

#define DECIMAL_SEPERATOR		'.'

int MILEX_prsdat(const char* data, uint32_t data_length, array* tokens);
int MILEX_packtokens(iterator* iter, enum mtoken_t begin, enum mtoken_t end, enum mtoken_t type, bool c, array* tokens);
int MILEX_proctokens(iterator* iter, mscript* script, mscope* scope);

static inline bool err(bool b, const char* text, ...)
{
  if (b)
    printf(text, "TODO:");
  return b;
}

#endif
