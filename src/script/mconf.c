#include "mconf.h"

#include "../utils/String.h"

#include <stdio.h>

static char* printloc(mtoken_l* location)
{
  string_buffer* line = string_buffer_new(128);

  uint32_t lcount = 1;
  bool err = false;
  for (uint32_t i = 0; i < location->slength; i++)
  {
    char c = location->source[i];
    
    if (lcount == location->lpos)
    {
      if (c == '\n')
	break;

      if (i >= location->start && i < location->end && !err)
      {
	string_buffer_append(line, "\e[1m\e[4m\e[31m");
	err = true;
      }else if ((i < location->start || i >= location->end) && err)
      {
	string_buffer_append(line, "\e[0m");
	err = false;
      }
      string_buffer_appendc(line, c);
    }

    if (c == '\n')
    {
      lcount++;
      continue;
    }
  }
  return string_buffer_extractd(line);
}

// TODO: string format

void ferr(mtoken_l* location, char* str)
{
  printf("\e[1m\e[37m%s\e[0m\e[35m [%i:%i]:\e[0m \e[31m%s\n\e[0m", location->file, location->lpos, location->cpos, str);
  printf("  [%i]  |  %s\n\n\e[0m", location->lpos, printloc(location));
}

void fwarn(mtoken_l* location, char* str)
{
  printf("\e[1m\e[37m%s\e[0m\e[35m [%i:%i]:\e[0m \e[33m%s\n\e[0m", location->file, location->lpos, location->cpos, str);
  printf("  [%i]  |  %s\n\n\e[0m", location->lpos, printloc(location));
}

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
