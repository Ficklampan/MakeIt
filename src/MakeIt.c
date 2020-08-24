#include "MakeIt.h"

#include "utils/FileUtils.h"
#include "utils/String.h"
#include "utils/Time.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "Config.h"

#include "Texts.h"
#include "script/mlexer.h"
#include "MakeItFunc.h"

static struct option long_options[] = {
  {"help", no_argument, 0, 'h'},
  {"version", no_argument, 0, 'v'},
  {"debug", no_argument, 0, 'd'},
  {"functions", no_argument, 0, 'f'},
  {"info", required_argument, 0, 'i'}
};

void pusage()
{
  printf("Usage: makeit [flags] [directory/file]\n");
  printf("Flags:\n");
  printf("  -d, --debug                  Enable debugging\n");
  printf("  -h, --help                   Prints this message and exit\n");
  printf("  -v, --version                Prints the current version\n");
  printf("  -m, --millis                 See how long it took to make it\n");
  printf("  -d, --debug                  Prints this message and exit\n");
  printf("  -f, --functions              Prints all functions\n");
  printf("  -i FUNC, --info=FUNC         Prints info about a function\n\n");
  printf("Report bugs at <https://github.com/Ficklampan/MakeIt/issues>\n");
}

void pversion()
{
  printf("Version: %s\n", MI_VERSION);
  printf("GNU GCC: %s\n", __VERSION__);
}

int main(int argc, char** argv)
{
  /* get options */
  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "dhmfi:", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 'd':
        CFG_mdebug(true);
      break;
      case 'm':
        CFG_mmillis(true);
      break;
      case 'h':
        pusage();
        return 0;
      break;
      case 'v':
      	pversion();
	return 0;
      break;
      case 'f':
        MIFUNC_usage("");
        return 0;
      break;
      case 'i':
        MIFUNC_usage(optarg);
        return 0;
      break;
      case '?':
        printf(":: unknown option '%c'.\n", (char) c);
        return 0;
      break;
      default:
        abort();
      break;
    }
  }

  /* init */
  uint64_t start_millis = tmillis();

  makeit_project* project = (makeit_project*) calloc(sizeof(makeit_project), 1);

  /* default filepath */
  char* filepath = "./init.makeit";

  /* if a filepath was specified then use that */
  if (optind < argc)
    filepath = argv[optind];

  if (!fsuexist(filepath))
  {
    printf(":: file not found `%s`.\n", filepath);
    return 1;
  }

  /* init directory and filepath */
  project->directory = strpathfix(strdir(filepath));
  project->filepath = filepath;

  fsumkd(strjoin(project->directory, "/MakeItFiles"));
  if (MI_procfile(project, project->filepath) != 1)
  {
    printf("\e[31m:: errors occurred while parsing file `%s`.\e[0m\n", project->filepath);
    return 1;
  }
  uint64_t end_millis = tmillis();
  printf("==> MakeIt made it without errors");
  if (CFG_millis())
    printf(", in %i milliseconds!\n", (end_millis - start_millis));
  else
    printf("!\n");
  return 0;
}

int MI_initproj(makeit_project* project, char* name, char* version, char* lang)
{
  project->name = name;
  project->version = version;
  if (strcmp(lang, "c") == 0)
    project->lang = LANG_C;
  else if (strcmp(lang, "c++") == 0 || strcmp(lang, "cpp") == 0 || strcmp(lang, "cxx") == 0)
    project->lang = LANG_CPP;
  else
  {
    printf(":: unknown language `%s`.\n", lang);
    return 0;
  }

  project->vars = (map*) calloc(sizeof(map), 1);
  project->libs = (array*) calloc(sizeof(array), 1);
  project->incs = (array*) calloc(sizeof(array), 1);
  project->ldirs = (array*) calloc(sizeof(array), 1);
  project->idirs = (array*) calloc(sizeof(array), 1);
  project->definitions = (array*) calloc(sizeof(array), 1);
  map_init(project->vars, 8);
  array_init(project->libs, 8);
  array_init(project->incs, 8);
  array_init(project->ldirs, 8);
  array_init(project->idirs, 8);
  array_init(project->definitions, 8);
  return 1;
}

int MI_procdat(makeit_project* project, const char* data, uint32_t data_length, const char* directory)
{
  array* tokens = (array*) calloc(sizeof(array), 1);
  array_init(tokens, 64);
  if (MILEX_prsdat(data, data_length, tokens) != 1)
    return 0;
  return 1;
}

int MI_procfile(makeit_project* project, const char* filepath)
{
  const char* directory = strdir(filepath);
  uint32_t length;
  uint8_t* data = fsurd(filepath, &length);
  if (CFG_debug())
    printf("==> [debug] file[%s]\n", filepath);
  return MI_procdat(project, (char*) data, length, directory);
}

static const char VAR_SEPARATOR = ' ';
int MI_procval(makeit_project* project, char** str, const char* directory)
{
  if (project->vars == NULL)
    return 1;

  if (strcmp(*str, "none") == 0)
  {
    *str[0] = '\0';
    return 1;
  }

  /* iterate over all available variables */
  for (uint32_t i = 0; i < project->vars->used; i++)
  {
    char* element_key = project->vars->keys[i];
    array* element_value = (array*) project->vars->values[i];

    /* if value is empty: ignore */
    if (element_value->used == 0)
      continue;

    /* create pointer string of key */
    char* key = strjoin(strjoin("$(", element_key), ")");

    /* check if variable has key */
    if (!strcnts(*str, key))
      continue;

    /* converting variable to string */
    string_buffer* value_str = (string_buffer*) calloc(sizeof(string_buffer), 1);
    string_buffer_init(value_str, 512);
    for (uint32_t i = 0; i < element_value->used; i++)
    {
      string_buffer_append(value_str, element_value->values[i]);
      if (i < element_value->used - 1)
        string_buffer_appendc(value_str, VAR_SEPARATOR);
    }
    /* replace all variable pointers to the variable */
    *str = strreplace(*str, key, value_str->str);
  }

  /* default variables */
  *str = strreplace(*str, "$(current_dir)", directory);
  *str = strreplace(*str, "$(project_dir)", project->directory);
  *str = strreplace(*str, "$(project_name)", project->name);
  return 1;
}
