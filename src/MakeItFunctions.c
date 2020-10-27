#include "MakeItFunctions.h"

#include "MakeFile.h"

#include "Config.h"
#include "Texts.h"

#include <me/mestr.h>

#include <fnmatch.h>

#include <stdio.h>
#include <stdlib.h>

static const char* FUNC_PROJECT_INFO        = "Specifying the project's name, version and language";
static const char* FUNC_DEFINE_INFO         = "Add definitions to the compiler (like '#define SOMETHING' in C)";
static const char* FUNC_LIBRARY_INFO        = "Add libraries or 'pkg-configs' to the compiler";
static const char* FUNC_INCLUDE_INFO        = "Add includes or 'pkg-configs' to the compiler";
static const char* FUNC_LIBRARY_DIR_INFO    = "Add library directories to the compiler";
static const char* FUNC_INCLUDE_DIR_INFO    = "Add include directories to the compiler";
static const char* FUNC_MAKEFILE_INFO       = "Generates a GNU Makefile";

static const char* FUNC_PROJECT_INFO_FULL   = "  param[0]: name[string] (project name)\n  param[1]: version[string]\n  param[1]: language[lang enum] (programming language)\n";
static const char* FUNC_DEFINE_INFO_FULL    = "  param[0]: project instance\n  param[+1]: name[string]\n";
static const char* FUNC_LIBRARY_INFO_FULL    = "  param[0]: project instance\n  param[+1]: library[string] (compiler flag '-l<lib>'.)\n";
static const char* FUNC_INCLUDE_INFO_FULL    = "  param[0]: project instance\n  param[+1]: include[string] (compiler flag '-i<incl>'.)\n";
static const char* FUNC_LIBRARY_DIR_INFO_FULL    = "  param[0]: project instance\n  param[+1]: library directory[string] (directory to search for libraries)\n";
static const char* FUNC_INCLUDE_DIR_INFO_FULL    = "  param[0]: project instance\n  param[+1]: include directory[string] (directory with headers or something)\n";
static const char* FUNC_MAKEFILE_INFO_FULL  = "  param[0]: project instance\n  param[1]: flags[string] (compiler flags)\n  param[2]: sources[string array] (a string of source files)\n  param[3]: headers (a string of header files)\n";

void MIFUNC_usage(const char* func)
{
  if (strempty(func))
  {
    printf("Functions:\n");
    printf("  'project'                    %s\n", FUNC_PROJECT_INFO);
    printf("  'define'                     %s\n", FUNC_DEFINE_INFO);
    printf("  'library'                    %s\n", FUNC_LIBRARY_INFO);
    printf("  'include'                    %s\n", FUNC_INCLUDE_INFO);
    printf("  'library_dir'                %s\n", FUNC_LIBRARY_DIR_INFO);
    printf("  'include_dir'                %s\n", FUNC_INCLUDE_DIR_INFO);
    printf("  'makefile'                   %s\n\n", FUNC_MAKEFILE_INFO);
    printf("Use '-i <function>' for more info about a function.\n");
  }else
  {
    if (strcmp(func, "project") == 0) printf("project:\n  %s\n\nUsage:\n%s\n", FUNC_PROJECT_INFO, FUNC_PROJECT_INFO_FULL);
    else if (strcmp(func, "define") == 0) printf("define:\n  %s\n\nUsage:\n%s\n", FUNC_DEFINE_INFO, FUNC_DEFINE_INFO_FULL);
    else if (strcmp(func, "library") == 0) printf("library:\n  %s\n\nUsage:\n%s\n", FUNC_LIBRARY_INFO, FUNC_LIBRARY_INFO_FULL);
    else if (strcmp(func, "include") == 0) printf("include:\n  %s\n\nUsage:\n%s\n", FUNC_INCLUDE_INFO, FUNC_INCLUDE_INFO_FULL);
    else if (strcmp(func, "library_dir") == 0) printf("library_dir:\n  %s\n\nUsage:\n%s\n", FUNC_LIBRARY_DIR_INFO, FUNC_LIBRARY_DIR_INFO_FULL);
    else if (strcmp(func, "include_dir") == 0) printf("include_dir:\n  %s\n\nUsage:\n%s\n", FUNC_INCLUDE_DIR_INFO, FUNC_INCLUDE_DIR_INFO_FULL);
    else if (strcmp(func, "makefile") == 0) printf("makefile:\n  %s\n\nUsage:\n%s\n", FUNC_MAKEFILE_INFO, FUNC_MAKEFILE_INFO_FULL);
    else
      printf(":: unknown function `%s`. use '-f' to list all functions.", func);
  }
}

int MIFUNC_print(struct mi_script* script, struct array* args)
{
  struct array* arg = args->entries[0];

  for (uint32_t i = 0; i < arg->size; i++)
  {
    printf("%s", arg->entries[i]);
    if (i < arg->size - 1)
      printf(" ");
  }

  printf("\n");

  return 1;
}

int MIFUNC_system(struct mi_script* script, struct array* args)
{
  for (uint32_t i = 0; i < args->size; i++)
  {
    struct array* arg = args->entries[i];
    for (uint32_t j = 0; j < arg->size; j++)
      system(arg->entries[j]);
  }

  return 1;
}

extern struct makeit_project project;

int MIFUNC_project(struct mi_script* script, struct array* args)
{
  struct array* arg = args->entries[0];

  project.name = arg->entries[0];

  return 1;
}

int MIFUNC_version(struct mi_script* script, struct array* args)
{
  struct array* arg = args->entries[0];

  project.version = arg->entries[0];

  return 1;
}

int MIFUNC_language(struct mi_script* script, struct array* args)
{
  for (uint32_t i = 0; i < args->size; i++)
  {
    struct array* arg = args->entries[i];
    mearr_push_all(project.language, arg);
  }

  return 1;
}

int MIFUNC_build_dir(struct mi_script* script, struct array* args)
{
  struct array* arg = args->entries[0];

  project.build_dir = me_file_new(arg->entries[0]);

  return 1;
}

int MIFUNC_compiler(struct mi_script* script, struct array* args)
{
  struct array* arg = args->entries[0];

  project.compiler = arg->entries[0];

  return 1;
}

int MIFUNC_script(struct mi_script* script, struct array* args)
{
  struct array* var = args->entries[0];

  for (uint32_t i = 0; i < var->size; i++)
  {
    if (MI_procfile(me_file_new(var->entries[i]), script) != 1)
      return 0;
  }
  return 1;
}

int MIFUNC_makefile(struct mi_script* script, struct array* args)
{
  struct array* arg_flags = args->entries[0];
  struct array* arg_sources = args->entries[1];
  struct array* arg_headers = args->entries[2];

  if (MI_makefile("./Makefile", arg_flags, arg_sources, arg_headers, &project) != 1)
    return 0;

  return 1;
}

int MIFUNC_define(struct mi_script* script, struct array* args)
{
  for (uint32_t i = 0; i < args->size; i++)
  {
    struct array* arg = args->entries[i];
    for (uint32_t j = 0; j < arg->size; j++)
      mearr_push(project.definitions, arg->entries[j]);
  }
  return 1;
}

int MIFUNC_library(struct mi_script* script, struct array* args)
{
  for (uint32_t i = 0; i < args->size; i++)
  {
    struct array* arg = args->entries[i];
    for (uint32_t j = 0; j < arg->size; j++)
      mearr_push(project.libs, arg->entries[j]);
  }
  return 1;
}

int MIFUNC_include(struct mi_script* script, struct array* args)
{
  for (uint32_t i = 0; i < args->size; i++)
  {
    struct array* arg = args->entries[i];
    for (uint32_t j = 0; j < arg->size; j++)
      mearr_push(project.incs, arg->entries[j]);
  }
  return 1;
}

int MIFUNC_library_dir(struct mi_script* script, struct array* args)
{
  for (uint32_t i = 0; i < args->size; i++)
  {
    struct array* arg = args->entries[i];
    for (uint32_t j = 0; j < arg->size; j++)
      mearr_push(project.ldirs, arg->entries[j]);
  }
  return 1;
}

int MIFUNC_include_dir(struct mi_script* script, struct array* args)
{
  for (uint32_t i = 0; i < args->size; i++)
  {
    struct array* arg = args->entries[i];
    for (uint32_t j = 0; j < arg->size; j++)
      mearr_push(project.idirs, arg->entries[j]);
  }
  return 1;
}

int MIFUNC_search(struct mi_script* script, struct array* args)
{
  struct array* search_dir_arg = args->entries[0];
  struct array* pattern_arg = args->entries[1];
  struct array* output_arg = args->entries[2];

  for (uint32_t k = 0; k < search_dir_arg->size; k++)
  {
    struct file* dir = me_file_new(search_dir_arg->entries[k]);

    struct array* files = mearr_new(64);
    me_file_list(dir, files);

    for (uint32_t i = 0; i < files->size; i++)
    {
      struct file* file = files->entries[i];
      bool match = false;

      for (uint32_t j = 0; j < pattern_arg->size; j++)
      {
	if (fnmatch(pattern_arg->entries[j], file->path, FNM_PERIOD) != FNM_NOMATCH)
	  match = true;
      }

      if (match)
	mearr_push(output_arg, file->path);
    }
  }

  return 1;
}
