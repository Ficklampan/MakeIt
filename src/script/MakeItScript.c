#include "MakeItScript.h"

#include <me/mestr.h>

#include <stdlib.h>

struct mi_script* MI_script_new(struct map* variables, struct map* functions)
{
  struct mi_script* script = malloc(sizeof(struct mi_script));
  script->variables = variables;
  script->functions = functions;
  return script;
}

struct mi_function* MI_function_new(int (*func) (struct mi_script*, struct array*))
{
  struct mi_function* function = malloc(sizeof(struct mi_function));
  function->func = func;
  return function;
}

struct array* MI_variable_new(char* str)
{
  struct array* v = mearr_new(4);
  mearr_push(v, str);
  return v;
}
