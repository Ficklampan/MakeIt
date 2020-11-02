#ifndef TIME_HPP
  #define TIME_HPP

#include <cstdint>
#include <sys/time.h>

static inline uint64_t tmillis()
{
  struct timeval time;
  gettimeofday(&time, nullptr);
  return time.tv_usec;
}

#endif
