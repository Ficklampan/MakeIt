#include "MakeItFunc.h"

#include "MakeFile.h"

#include "utils/String.h"
#include "utils/FileUtils.h"

#include "texts.h"

#include <stdio.h>
#include <stdlib.h>

int makeit_process_functions(makeit_project* project, const char* func, const array* elements, const char* directory)
{
  #ifdef DEBUG
    printf("[DEBUG] ==> processing function: %s\n", func);
  #endif
  if (strcmp(func, "project") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    makeit_init_project(project, elements->values[0]);
  }else if (strcmp(func, "directory") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_NO_DIR_SPEC, "+1", elements->used, func);
      return 0;
    }

  }else if (strcmp(func, "variable") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_NO_VAR_NAME_SPECIFIED, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
    {
      array* value = (array*) calloc(sizeof(array), 1);
      array_init(value, 8);
      map_push(project->vars, elements->values[i], value);
    }
  }else if (strcmp(func, "append") == 0)
  {
    if (elements->used < 2)
    {
      printf(ERR_TOO_FEW_ARGS, "+2", elements->used);
      return 0;
    }
    array* value = (array*) map_pull(project->vars, elements->values[0]);
    if (value == NULL)
    {
      printf(ERR_VAR_NOT_FOUND, elements->values[0], func);
      return 0;
    }

    /* appending all elements in the array to the target variable */
    for (uint32_t i = 1; i < elements->used; i++)
      array_push(value, elements->values[i]);
  }else if (strcmp(func, "cout") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
      printf("[cout] ==> %s\n", (char*) elements->values[i]);
  }else if (strcmp(func, "system") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
      system(elements->values[i]);
  }else if (strcmp(func, "include") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
      array_push(project->include_paths, elements->values[i]);
  }else if (strcmp(func, "library") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
      array_push(project->lib_paths, elements->values[i]);
  }else if (strcmp(func, "define") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
      array_push(project->definitions, elements->values[i]);
  }else if (strcmp(func, "search") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+2", elements->used, func);
      return 0;
    }
    array* value = (array*) map_pull(project->vars, elements->values[0]);
    if (value == NULL)
    {
      printf(ERR_VAR_NOT_FOUND, elements->values[0], func);
      return 0;
    }

    array_clear(value);
    for (uint32_t i = 1; i < elements->used; i++)
      file_utils_find(strdir(elements->values[i]), strfname(elements->values[i]), value, true);
  }else if (strcmp(func, "makefile") == 0)
  {
    if (elements->used < 4)
    {
      printf(ERR_TOO_FEW_ARGS, "4", elements->used, func);
      return 0;
    }
    printf(INFO_CONSTRUCT_MAKEFILE);

    char* makefile_path = strjoin(strjoin(project->directory, "/"), "Makefile");
    char* info_log;

    if (!make_makefile(project->name, project->directory, makefile_path, elements->values[0],
      elements->values[1],
      elements->values[2],
      elements->values[3],
      project->include_paths,
      project->lib_paths,
      project->definitions,
      "MakeItFiles", &info_log))
    {
      printf(ERR_MAKEFILE_FAILED, info_log);
      return 0;
    }
    printf(INFO_CONSTRUCT_MAKEFILE_DONE);
  }
  return 1;
}
