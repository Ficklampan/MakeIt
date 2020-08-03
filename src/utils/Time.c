#include "Time.h"

#include <sys/time.h>

uint64_t time_millis()
{
  struct timeval time;
  gettimeofday(&time, NULL);
  return time.tv_usec;
}
