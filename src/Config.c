#include "Config.h"

static bool conf_debug = false;
static bool conf_millis = false;

bool CFG_debug()
{
  return conf_debug;
}
void CFG_mdebug(bool debug)
{
  conf_debug = debug;
}

bool CFG_millis()
{
  return conf_millis;
}

void CFG_mmillis(bool millis)
{
  conf_millis = millis;
}
