#include "Config.h"

static bool conf_trace = false;
static bool conf_debug = false;

bool config_trace()
{
  return conf_trace;
}
void config_set_trace(bool trace)
{
  conf_trace = trace;
}

bool config_debug()
{
  return conf_debug;
}
void config_set_debug(bool debug)
{
  conf_debug = debug;
}
