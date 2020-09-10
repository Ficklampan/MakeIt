#include "mconf.h"

#include "../utils/String.h"

#include <stdarg.h>
#include <stdio.h>

// TODO: warning colors
static int printloc(mtoken_l* location, char* format, char** out)
{
  string_buffer* line = string_buffer_new(128);

  uint32_t lcount = 1;
  for (uint32_t i = 0; i < location->slength; i++)
  {
    char c = location->source[i];
    
    if (lcount == location->lpos)
    {
      if (c == '\n')
	break;

      if (i == location->start)
	string_buffer_append(line, format);
      else if (i == location->end)
	string_buffer_append(line, "\e[0m");
      string_buffer_appendc(line, c);
    }

    if (c == '\n')
    {
      lcount++;
      continue;
    }
  }

  *out = string_buffer_extractd(line);
  return 1;
}

// TODO: string format

void ferr(mtoken_l* location, const char* format, ...)
{
  char* line = NULL;

  printloc(location, "\e[4m\e[31m", &line);

  printf("\e[1m\e[37m%s\e[0m\e[35m [%i:%i]:\e[0m \e[31m", location->file, location->lpos, location->cpos);

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  printf("\n\e[0m  [%i]  | %s\n\n\e[0m", location->lpos, line);
}

void fwarn(mtoken_l* location, const char* format, ...)
{
  char* line = NULL;

  printloc(location, "\e[4m\e[33m", &line);

  printf("\e[1m\e[37m%s\e[0m\e[35m [%i:%i]:\e[0m \e[33m", location->file, location->lpos, location->cpos);

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  printf("\n\e[0m  [%i]  | %s\n\n\e[0m", location->lpos, line);
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
