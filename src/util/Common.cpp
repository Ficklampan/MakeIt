#include "Common.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>

uint64_t tmillis()
{
  struct timeval time;
  gettimeofday(&time, NULL);
  return time.tv_usec;
}

int question(int preset, const char* format, va_list args)
{
  vprintf(format, args);

  if (preset)
    printf(" %s ", _("[Y/n]"));
  else
    printf(" %s ", _("[y/N]"));

  char response[64];
  fgets(response, 64, stdin);

  uint8_t len = strlen(response);

  if (len > 0)
  {
    if (strcasecmp(_("Y"), response) == 0 || strcasecmp(_("YES"), response) == 0)
      return 1;
    else if (strcasecmp(_("N"), response) == 0 || strcasecmp(_("NO"), response) == 0)
      return 0;
  }
  return preset;
}

int yesno(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  int response = question(1, format, args);
  va_end(args);
  return response;
}

int noyes(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  int response = question(0, format, args);
  va_end(args);
  return response;
}
