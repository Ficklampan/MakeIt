#ifndef MI_SCRIPT_H
  #define MI_SCRIPT_H

#include <me/memap.h>
#include <me/mearr.h>

struct mi_script {

  struct map* variables;
  struct map* functions;

};

struct mi_function {
  int (*func) (struct mi_script*, struct array*);
};

struct mi_script* MI_script_new(struct map* variables, struct map* functions);
struct mi_function* MI_function_new(int (*func) (struct mi_script*, struct array*));
struct array* MI_variable_new(char* str);

#endif
