#ifndef MI_CONFIG_HPP
  #define MI_CONFIG_HPP

#define MIDEBUG(l, ...) if (config.debug >= l) { /*printf("'%s'[%u]: ", __FILE__, __LINE__);*/ printf(__VA_ARGS__); }

#include <string>

struct Config {
  std::string file;
  bool print_all;
  bool block_all;
  uint8_t debug;
};

#endif
