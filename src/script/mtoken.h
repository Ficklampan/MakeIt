#ifndef MI_TOKEN_H
  #define MI_TOKEN_H

#include "mconf.h"
#include "mtype.h"

#include "../utils/String.h"

#include <stdio.h>
#include <stdlib.h>

static inline mtoken* ntoken(enum mtoken_t type, void* value, mtoken_l* location)
{
  mtoken* token = (mtoken*) calloc(sizeof(mtoken), 1);
  token->type = type;
  token->value = value;
  token->location = location;
  return token;
}

static inline char* ttstr(enum mtoken_t type)
{
  switch (type)
  {
    case MTK_END_T: return "end"; break;
    case MTK_COMMA_T: return "comma"; break;
    case MTK_DOT_T: return "dot"; break;
    case MTK_GROUP_T: return "group"; break;
    case MTK_SCOPE_T: return "scope"; break;
    case MTK_CALL_T: return "call"; break;
    case MTK_LITERIAL_T: return "literial"; break;
    case MTK_VALUE_T: return "value"; break;
    case MTK_OPERATOR_T: return "operator"; break;
    default:
      return "?";
  }
}



#endif
