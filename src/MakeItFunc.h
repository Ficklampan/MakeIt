#ifndef MAKE_IT_FUNC_H
  #define MAKE_IT_FUNC_H

int makeit_process_functions(makeit_project* project, const char* func, const char** args, uint32_t argc)
{
  if (strcmp("project", func))
  {
    if (argc < 1)
    {
      printf(ERR_TOO_FEW_ARGS, func);
      return 0;
    }
    project = (makeit_project*) calloc(sizeof(makeit_project), 1);
    project->name = args[0];
    map_init(&project->vars, 4);
  }else if (strcmp("directory", func))
  {
    if (argc < 1)
    {
      printf(ERR_NO_DIR_SPEC, func);
      return 0;
    }

  }else if (strcmp("variable", func))
  {
    if (argc < 1)
    {
      printf(ERR_NO_VAR_NAME_SPECIFIED, func);
      return 0;
    }
    const char* value = "\0";
    for (uint32_t i = 0; i < argc; i++)
      map_push(&project->vars, args[i], value);
  }else if (strcmp("append", func))
  {
    if (argc < 2)
    {
      printf(ERR_TOO_FEW_ARGS, func);
      return 0;
    }
    char* value = (char*) map_pull(&project->vars, args[0]);
    value = strapnd(value, args[1]);
    map_push(&project->vars, args[0], (void*) value);
  }else if (strcmp("cout", func))
  {
    if (argc < 1)
    {
      printf(ERR_TOO_FEW_ARGS, func);
      return 0;
    }
    for (uint32_t i = 0; i < argc; i++)
      printf("%s\n", args[i]);
  }
  return 1;
}

#endif
