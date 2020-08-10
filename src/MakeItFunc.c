#include "MakeItFunc.h"

#include "MakeFile.h"

#include "Config.h"

#include "utils/String.h"
#include "utils/FileUtils.h"

#include "Texts.h"

#include <stdio.h>
#include <stdlib.h>

static const char* FUNC_PROJECT_INFO        = "Specifying the project's name and language";
static const char* FUNC_INCLUDE_INFO        = "Include external MakeIt scripts";
static const char* FUNC_VARIABLE_INFO       = "Creating variable(s) placeholder";
static const char* FUNC_APPEND_INFO         = "Append data to a variable";
static const char* FUNC_COUT_INFO           = "Print stuff to the console";
static const char* FUNC_SYSTEM_INFO         = "Execute system commands";
static const char* FUNC_DEFINE_INFO         = "Add definitions to the compiler (like '#define SOMETHING' in C)";
static const char* FUNC_SEARCH_INFO         = "Searching for files with a pattern and adding it to the specified variable";
static const char* FUNC_VERIFY_INFO         = "Check if files exists";
static const char* FUNC_DEPEND_INFO         = "Add dependencies using URIs / URLs";
static const char* FUNC_LIBRARY_INFO        = "Add libraries to the compiler";
static const char* FUNC_LIBRARY_DIR_INFO    = "Add library directories to the compiler";
static const char* FUNC_INCLUDE_DIR_INFO    = "Add include directories to the compiler";
static const char* FUNC_MAKEFILE_INFO       = "Generates a GNU Makefile";

static const char* FUNC_PROJECT_INFO_FULL   = "  param[0]: name (project name)\n  param[1]: language[C/C++] (programming language)\n  param[2]: version (project version)\n  param[3] (*optinal): dependency directory\n";
static const char* FUNC_INCLUDE_INFO_FULL   = "  param[+0]: directory/file (directory with a MakeIt script)\n";
static const char* FUNC_VARIABLE_INFO_FULL  = "  param[+0]: name (variable name)";
static const char* FUNC_APPEND_INFO_FULL    = "  param[0]: variable (what variable to append)\n  param[+1]: data\n";
static const char* FUNC_COUT_INFO_FULL      = "  param[+0]: message\n";
static const char* FUNC_SYSTEM_INFO_FULL    = "  param[+0]: command (system command)\n";
static const char* FUNC_DEFINE_INFO_FULL    = "  param[+0]: name\n";
static const char* FUNC_SEARCH_INFO_FULL    = "  param[0]: variable (what variable to append found files)\n  param[+1]: pattern (filepath pattern e.g: 'src/include/*.h')\n";
static const char* FUNC_VERIFY_INFO_FULL    = "  param[+0]: file(s) (relative/absolute filepath)\n";
static const char* FUNC_DEPEND_INFO_FULL    = "  param[+0]: dependency (URI / URL)\n";
static const char* FUNC_LIBRARY_INFO_FULL    = "  param[+0]: library (compiler flag '-l<lib>')\n";
static const char* FUNC_LIBRARY_DIR_INFO_FULL    = "  param[+0]: library directory (directory to search for libraries)\n";
static const char* FUNC_INCLUDE_DIR_INFO_FULL    = "  param[+0]: include directory (directory with headers or something)\n";
static const char* FUNC_MAKEFILE_INFO_FULL  = "  param[0]: flags (compiler flags)\n  param[1]: sources (a string of source files)\n  param[2]: headers (a string of header files)\n";

void usage_function(const char* func)
{
  if (strempty(func))
  {
    printf("Functions:\n");
    printf("  'project'                    %s\n", FUNC_PROJECT_INFO);
    printf("  'include'                    %s\n", FUNC_INCLUDE_INFO);
    printf("  'variable'                   %s\n", FUNC_VARIABLE_INFO);
    printf("  'append'                     %s\n", FUNC_APPEND_INFO);
    printf("  'cout'                       %s\n", FUNC_COUT_INFO);
    printf("  'system'                     %s\n", FUNC_SYSTEM_INFO);
    printf("  'define'                     %s\n", FUNC_DEFINE_INFO);
    printf("  'search'                     %s\n", FUNC_SEARCH_INFO);
    printf("  'verify'                     %s\n", FUNC_VERIFY_INFO);
    printf("  'dependencies'               %s\n", FUNC_DEPEND_INFO);
    printf("  'library'                    %s\n", FUNC_LIBRARY_INFO);
    printf("  'library_dir'                %s\n", FUNC_LIBRARY_DIR_INFO);
    printf("  'include_dir'                %s\n", FUNC_INCLUDE_DIR_INFO);
    printf("  'makefile'                   %s\n\n", FUNC_MAKEFILE_INFO);
    printf("Use '-i <function>' for more info about a function.\n");
  }else
  {
    if (strcmp(func, "project") == 0) printf("project:\n  %s\n\nUsage:\n%s\n", FUNC_PROJECT_INFO, FUNC_PROJECT_INFO_FULL);
    else if (strcmp(func, "include") == 0) printf("directory:\n  %s\n\nUsage:\n%s\n", FUNC_INCLUDE_INFO, FUNC_INCLUDE_INFO_FULL);
    else if (strcmp(func, "variable") == 0) printf("variable:\n  %s\n\nUsage:\n%s\n", FUNC_VARIABLE_INFO, FUNC_VARIABLE_INFO_FULL);
    else if (strcmp(func, "append") == 0) printf("append:\n  %s\n\nUsage:\n%s\n", FUNC_APPEND_INFO, FUNC_APPEND_INFO_FULL);
    else if (strcmp(func, "cout") == 0) printf("cout:\n  %s\n\nUsage:\n%s\n", FUNC_COUT_INFO, FUNC_COUT_INFO_FULL);
    else if (strcmp(func, "system") == 0) printf("system:\n  %s\n\nUsage:\n%s\n", FUNC_SYSTEM_INFO, FUNC_SYSTEM_INFO_FULL);
    else if (strcmp(func, "define") == 0) printf("define:\n  %s\n\nUsage:\n%s\n", FUNC_DEFINE_INFO, FUNC_DEFINE_INFO_FULL);
    else if (strcmp(func, "search") == 0) printf("search:\n  %s\n\nUsage:\n%s\n", FUNC_SEARCH_INFO, FUNC_SEARCH_INFO_FULL);
    else if (strcmp(func, "verify") == 0) printf("verify:\n  %s\n\nUsage:\n%s\n", FUNC_VERIFY_INFO, FUNC_VERIFY_INFO_FULL);
    else if (strcmp(func, "dependencies") == 0) printf("dependencies:\n  %s\n\nUsage:\n%s\n", FUNC_DEPEND_INFO, FUNC_DEPEND_INFO_FULL);
    else if (strcmp(func, "library") == 0) printf("library:\n  %s\n\nUsage:\n%s\n", FUNC_LIBRARY_INFO, FUNC_LIBRARY_INFO_FULL);
    else if (strcmp(func, "library_dir") == 0) printf("library_dir:\n  %s\n\nUsage:\n%s\n", FUNC_LIBRARY_DIR_INFO, FUNC_LIBRARY_DIR_INFO_FULL);
    else if (strcmp(func, "include_dir") == 0) printf("include_dir:\n  %s\n\nUsage:\n%s\n", FUNC_INCLUDE_DIR_INFO, FUNC_INCLUDE_DIR_INFO_FULL);
    else if (strcmp(func, "makefile") == 0) printf("makefile:\n  %s\n\nUsage:\n%s\n", FUNC_MAKEFILE_INFO, FUNC_MAKEFILE_INFO_FULL);
    else
      printf(":: unknown function `%s`. use '-f' to list all functions.", func);
  }
}

int makeit_process_functions(makeit_project* project, const char* func, const array* elements, const char* directory)
{
  if (strcmp(func, "project") == 0)
  {
    if (elements->used < 3)
    {
      printf(ERR_TOO_FEW_ARGS, "+3", elements->used, func);
      return 0;
    }
    if (elements->used > 3)
      project->deps_directory = elements->values[2];
    if (makeit_init_project(project, elements->values[0], elements->values[1], elements->values[2]) != 1)
      return 0;
  }else if (strcmp(func, "include") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_NO_DIR_SPEC, "+1", elements->used, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
    {
      char* filepath = strjoin(strpathfix(elements->values[i]), "/MakeIt.txt");
      if (makeit_parse_file(project, filepath) != 1)
      {
        printf(ERR_FAILED_PARSING, filepath);
        return 0;
      }
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
      file_utils_find(strdir(elements->values[i]), elements->values[i], value, true);
  }else if (strcmp(func, "dependencies") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    if (project->deps_directory == NULL)
    {
      printf(ERR_DEPS_DIR_NOT_SPECIFIED);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
    {
      char* dependency = elements->values[i];

    }
  }else if (strcmp(func, "attach") == 0)
  {
    if (elements->used < 2)
    {
      printf(ERR_TOO_FEW_ARGS, "+2", elements->used, func);
      return 0;
    }

    int type;
    if (strcmp(elements->values[0], "MAKEIT") == 0) type = 0;
    else if (strcmp(elements->values[0], "CMAKE") == 0) type = 1;
    else if (strcmp(elements->values[0], "PREMAKE") == 0) type = 2;
    else
    {
      printf(ERR_UNKNOWN, "type", elements->values[0], func);
      return 0;
    }

    for (uint32_t i = 1; i < elements->used; i++)
    {
    }
  }else if (strcmp(func, "library") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
      array_push(project->libs, elements->values[i]);
  }else if (strcmp(func, "library_dir") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
      array_push(project->ldirs, elements->values[i]);
  }else if (strcmp(func, "include_dir") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    for (uint32_t i = 0; i < elements->used; i++)
      array_push(project->idirs, elements->values[i]);
  }else if (strcmp(func, "verify") == 0)
  {
    if (elements->used < 1)
    {
      printf(ERR_TOO_FEW_ARGS, "+1", elements->used, func);
      return 0;
    }
    uint32_t total = 0;
    array* missing = (array*) calloc(sizeof(array), 1);
    array_init(missing, 8);
    for (uint32_t i = 0; i < elements->used; i++)
    {
      array* files = strsplit(elements->values[i], ' ');
      for (uint32_t j = 0; j < files->used; j++)
      {
        if (!file_utils_exists(files->values[j]))
          array_push(missing, files->values[j]);
        total++;
      }
    }
    uint32_t found = total - missing->used;

    // add some nice colors
    if (missing->used > 0) printf("\e[33m");
    else printf("\e[32m");
    
    printf("==> [%i/%i] files found\e[0m\n", found, total);
    if (missing->used > 0)
    {
      printf(":: missing:\n");
      for (uint32_t i = 0; i < missing->used; i++)
        printf("  `%s`\n", (char*) missing->values[i]);
      return 0;
    }
  }else if (strcmp(func, "makefile") == 0)
  {
    if (elements->used < 3)
    {
      printf(ERR_TOO_FEW_ARGS, "3", elements->used, func);
      return 0;
    }else if (elements->used > 3)
    {
      printf(ERR_TOO_MANY_ARGS, "3", elements->used, func);
      return 0;
    }
    char* makefile_path = strjoin(project->directory, "/Makefile");
    char* info_log;

    if (!make_makefile(project->name, project->version, project->directory, makefile_path, 
      elements->values[0], // cflags
      strsplit(elements->values[1], ' '), // sources
      strsplit(elements->values[2], ' '), // headers
      project->libs, // libs
      project->idirs, // include directories
      project->ldirs, // library directories
      project->definitions, // definitions
      project->lang == LANG_CPP ? "c++" : "c",
      strjoin(project->directory, "/MakeItFiles"), &info_log))
    {
      printf(ERR_MAKEFILE_FAILED, info_log);
      return 0;
    }
  }
  return 1;
}
