#ifndef CONFIG_H
  #define CONFIG_H

#include "utils/Type.h"

bool config_trace();
void config_set_trace(bool trace);

bool config_debug();
void config_set_debug(bool debug);

bool config_millis();
void config_set_millis(bool millis);

#endif
