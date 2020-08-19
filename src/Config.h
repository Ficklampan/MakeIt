#ifndef CONFIG_H
  #define CONFIG_H

#include "utils/Type.h"

/* 'm...' = 'modify' */

bool CFG_debug();
void CFG_mdebug(bool debug);

bool CFG_millis();
void CFG_mmillis(bool millis);

#endif
