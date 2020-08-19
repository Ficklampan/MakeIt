#ifndef TIME_H
  #define TIME_H

#include "Type.h"

#include <sys/time.h>

static inline uint64_t tmillis()
{
  struct timeval time;
  gettimeofday(&time, NULL);
  return time.tv_usec;
}

#endif
