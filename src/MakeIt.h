#ifndef MAKEIT_H
  #define MAKEIT_H

#define MI_VERSION		"2020-08-18"

#include <me/mearr.h>
#include <me/memap.h>
#include <me/mefil.h>

#include "script/MakeItScript.h"

enum lang_t {
  LANG_UNK, LANG_C, LANG_CPP
};

struct makeit_project {
  char* name;

  char* version;
  struct array* language;

  struct file* build_dir;
  char* compiler;

  struct array* configs;
  struct array* libs;
  struct array* incs;
  struct array* ldirs;
  struct array* idirs;
  struct array* definitions;
};

int MI_procdat(struct file* file, void* data, uint32_t size, struct mi_script* script);
int MI_procfile(struct file* file, struct mi_script* script);

#endif
