#ifndef MAKE_IT_FUNC_H
  #define MAKE_IT_FUNC_H

#include "script/MakeItScript.h"

void MIFUNC_usage(const char* func);

int MIFUNC_print(struct mi_script* script, struct array* args);
int MIFUNC_system(struct mi_script* script, struct array* args);

int MIFUNC_project(struct mi_script* script, struct array* args);
int MIFUNC_version(struct mi_script* script, struct array* args);
int MIFUNC_language(struct mi_script* script, struct array* args);
int MIFUNC_build_dir(struct mi_script* script, struct array* args);
int MIFUNC_compiler(struct mi_script* script, struct array* args);
int MIFUNC_script(struct mi_script* script, struct array* args);
int MIFUNC_makefile(struct mi_script* script, struct array* args);
int MIFUNC_define(struct mi_script* script, struct array* args);
int MIFUNC_library(struct mi_script* script, struct array* args);
int MIFUNC_include(struct mi_script* script, struct array* args);
int MIFUNC_library_dir(struct mi_script* script, struct array* args);
int MIFUNC_include_dir(struct mi_script* script, struct array* args);
int MIFUNC_search(struct mi_script* script, struct array* args);

#endif
