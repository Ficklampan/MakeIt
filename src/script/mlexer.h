#ifndef MAKE_IT_LEXER_H
  #define MAKE_IT_LEXER_H

#include "mtype.h"
#include "mtoken.h"
#include "mfunctions.h"

#include "../utils/Type.h"
#include "../utils/Arrays.h"
#include "../utils/String.h"

#include <stdio.h>

#define EXPR_END			';'
#define MAX_STRING_LENGTH		4096
#define DECIMAL_SEPERATOR		'.'

int MILEX_makescript(mscript* script, const char* libs);
int MILEX_putfunc(mscript* script, char* name, mfunc* func);

int MILEX_maketokens(const char* data, uint32_t len, array* tokens, const char* file, mscript* script);
int MILEX_nexttoken(const char* data, uint32_t* index, uint32_t* lpos, uint32_t* cpos, uint32_t len, const char* file, mscript* script, mtoken* last_token, mtoken** token);

#endif
