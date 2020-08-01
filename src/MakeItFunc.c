#include "MakeItFunc.h"

#include "utils/String.h"
#include "texts.h"

#include "MakeFile.h"

#include <stdio.h>
#include <stdlib.h>

static const char END_WITH_SPACE = 1;

int makeit_process_functions(makeit_project* project, const char* func, const array* elements)
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
    for (uint32_t i = 0; i < elements->used; i++)
    {
      string_buffer* value = calloc(sizeof(string_buffer), 1);
      string_buffer_init(value, 512);
      map_push(project->vars, elements->values[i], value);
    }
  }else if (strcmp(func, "append") == 0)
  {
    if (elements->used < 2)
    {
      printf(ERR_TOO_FEW_ARGS, "+2", elements->used);
      return 0;
    }
    string_buffer* value = (string_buffer*) map_pull(project->vars, elements->values[0]);
    if (value == NULL)
    {
      printf(ERR_VAR_NOT_FOUND, elements->values[0], func);
      return 0;
    }

    /* appending all elements in the array to the target variable */
    for (uint32_t i = 1; i < elements->used; i++)
    {
      /* to append */
      char* apnd = (char*) elements->values[i];
      uint32_t len = strlen(apnd);

      for (uint32_t j = 0; j < len; j++)
        string_buffer_appendc(value, apnd[j]);

      if (END_WITH_SPACE == 1)
        string_buffer_appendc(value, ' ');
    }
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
  }else if (strcmp(func, "makefile") == 0)
  {
    if (elements->used < 4)
    {
      printf(ERR_TOO_FEW_ARGS, "4", elements->used, func);
      return 0;
    }
    printf(INFO_CONSTRUCT_MAKEFILE);

    char* makefile_path = strjoin(project->directory, "Makefile");
    char* info_log;

    uint32_t sources_length, headers_length, libs_length;
    const char** sources = strsplit(elements->values[1], ' ', &sources_length);
    const char** headers = strsplit(elements->values[2], ' ', &headers_length);
    const char** libs = strsplit(elements->values[3], ' ', &libs_length);

    array* sources_array = (array*) calloc(sizeof(array), 1);
    array* headers_array = (array*) calloc(sizeof(array), 1);
    array* libs_array = (array*) calloc(sizeof(array), 1);

    array_init(sources_array, sources_length);
    array_init(headers_array, headers_length);
    array_init(libs_array, libs_length);

    for (uint32_t i = 0; i < sources_length; i++) array_push(sources_array, sources[i]);
    for (uint32_t i = 0; i < headers_length; i++) array_push(headers_array, headers[i]);
    for (uint32_t i = 0; i < libs_length; i++) array_push(libs_array, libs[i]);

    if (!make_makefile(project->name, project->directory, makefile_path, elements->values[0],
      sources_array,
      headers_array,
      libs_array,
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
