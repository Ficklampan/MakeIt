#ifndef MAKEIT_COMMON_H
  #define MAKEIT_COMMON_H

#include <libintl.h>

#define _(s) gettext(s)

#include <stdint.h>
#include <sys/time.h>

uint64_t tmillis();
int question(char** answer, const char* format, ...);
int yesno(const char* format, ...);
int noyes(const char* format, ...);

#endif
