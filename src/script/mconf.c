#include "mconf.h"

char* wstr(unsigned int w)
{
  switch (w)
  {
    case W_CAST:
      return ":: error while casting type '%s' to '%s', [-Wcast].\n";
      break;
    case W_ASSIGN_CONST:
      return ":: can't assign const value, [-Wassign_const].\n'";
      break;
    default:
      return ":: unknown warning.\n";
  }
}
