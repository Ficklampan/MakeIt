#ifndef CONFIG_H
  #define CONFIG_H

#define MI_USE_32BIT_INTEGER
#define MI_USE_65BIT_FLOAT

#define W_CAST			0x01
#define W_ASSIGN_CONST		0x02

#define err(s, ...) { printf("\e[31m"); printf((s), ##__VA_ARGS__); printf("\e[0m"); }
#define warn(s, ...) { printf("\e[33m"); printf((s), ##__VA_ARGS__); printf("\e[0m"); }

#include "mtype.h"

void ferr(mtoken_l* location, const char* format, ...);
void fwarn(mtoken_l* location, const char* format, ...);

char* wstr(unsigned int w);

#endif
