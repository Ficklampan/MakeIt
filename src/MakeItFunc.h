#ifndef MAKE_IT_FUNC_H
  #define MAKE_IT_FUNC_H

int makeit_process_functions(makeit_project* project, const char* func, const array* elements)
{
  if (strcmp(func, "project") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, func);
      return 0;
    }
    makeit_init_project(project, elements->values[0]);
  }else if (strcmp(func, "directory") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_NO_DIR_SPEC, func);
      return 0;
    }

  }else if (strcmp(func, "variable") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_NO_VAR_NAME_SPECIFIED, func);
      return 0;
    }
    string_buffer* value = calloc(sizeof(string_buffer), 1);
    string_buffer_init(value, 512);
    for (uint32_t i = 0; i < elements->used; i++)
      map_push(project->vars, elements->values[i], value);
  }else if (strcmp(func, "append") == 0)
  {
    if (elements->used < 2)
    {
      printf(ERR_TOO_FEW_ARGS, func);
      return 0;
    }
    string_buffer* value = (string_buffer*) map_pull(project->vars, elements->values[0]);

    /* appending all elements in the array to the target variable */
    for (uint32_t i = 1; i < elements->used; i++)
    {
      /* to append */
      char* apnd = (char*) elements->values[i];
      uint32_t len = strlen(apnd);

      for (uint32_t j = 0; j < len; j++)
        string_buffer_append(value, apnd[j]);
    }
  }else if (strcmp(func, "cout") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
    {

      printf("[cout] ==> %s\n", elements->values[i]);
    }
  }
  return 1;
}

#endif
