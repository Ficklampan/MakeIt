#include "MakeIt.h"

#include "utils/FileUtils.h"
#include "utils/String.h"
#include "utils/Map.h"
#include "utils/Time.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "Config.h"
#include "MemPool.h"

#include "texts.h"
#include "MakeItParser.h"
#include "MakeItFunc.h"

static struct option long_options[] = {
  {"help", no_argument, 0, 'h'},
  {"trace", no_argument, 0, 't'},
  {"debug", no_argument, 0, 'd'},
  {"functions", no_argument, 0, 'f'},
  {"info", required_argument, 0, 'i'}
};

void usage()
{
  printf("Usage: makeit [flags] [directory/file]\n");
  printf("Flags:\n");
  printf("  -t, --trace                  Enable tracing\n");
  printf("  -d, --debug                  Enable debugging\n");
  printf("  -h, --help                   Prints this message and exit\n");
  printf("  -m, --millis                 See how long it took to make it\n");
  printf("  -d, --debug                  Prints this message and exit\n");
  printf("  -f, --functions              Prints all functions\n");
  printf("  -i FUNCTION, --info=FUNCTION\n                               Prints info about a function\n");
  printf("Report bugs at <https://github.com/Ficklampan/MakeIt/issues>\n");
}

int main(int argc, char** argv)
{
  /* get options */
  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "tdhmfi:", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 't':
        config_set_trace(true);
      break;
      case 'd':
        config_set_debug(true);
      break;
      case 'm':
        config_set_millis(true);
      break;
      case 'h':
        usage();
        return 0;
      break;
      case 'f':
        usage_function("");
        return 0;
      break;
      case 'i':
        usage_function(optarg);
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
  uint64_t start_millis = time_millis();

  MemPool* mem_pool = (MemPool*) calloc(sizeof(MemPool), 1);
  mem_pool_init(mem_pool, 1024);
  mem_pool_bind(mem_pool);

  makeit_project* project = (makeit_project*) mem_calloc(sizeof(makeit_project), 1);

  /* default filepath */
  char* filepath = "./MakeIt.txt";

  /* if a filepath was specified then use that */
  if (optind < argc)
    filepath = argv[optind];

  if (!file_utils_exists(filepath))
  {
    printf(":: file not found `%s`.\n", filepath);
    return 1;
  }

  /* init directory and filepath */
  project->directory = strpathfix(strdir(filepath));
  project->filepath = filepath;

  if (config_trace())
    printf("==> Parsing `%s`\n", project->filepath);
  file_utils_mkdir(strjoin(project->directory, "/MakeItFiles"));
  if (makeit_parse_file(project, project->filepath) != 1)
  {
    printf(":: errors occurred while parsing file `%s`.\n", project->filepath);
    mem_afree();
    return 1;
  }
  uint64_t end_millis = time_millis();
  printf("==> MakeIt made it without errors");
  if (config_millis())
    printf(", in %i milliseconds!\n", (end_millis - start_millis));
  else
    printf("!\n");
  mem_afree();
  return 0;
}

int makeit_init_project(makeit_project* project, char* name, char* lang)
{
  project->name = name;
  if (strcmp(lang, "c") == 0)
    project->lang = LANG_C;
  else if (strcmp(lang, "c++") == 0 || strcmp(lang, "cpp") == 0 || strcmp(lang, "cxx") == 0)
    project->lang = LANG_CPP;
  else
  {
    printf(":: unknown language `%s`.", lang);
    return 0;
  }

  project->vars = calloc(sizeof(map), 1);
  project->definitions = calloc(sizeof(array), 1);
  map_init(project->vars, 8);
  array_init(project->definitions, 8);
  return 1;
}

int makeit_parse_data(makeit_project* project, const char* data, const char* directory)
{
  array* tokens = makeit_parser_parse_data(data);
  for (uint32_t i = 0; i < tokens->used; i++)
  {
    func_element* elem = (func_element*) tokens->values[i];

    /* debug stuff */
    if (config_debug())
    {
      printf("==> [debug] function[%s]:\n", elem->name);
      for (uint32_t j = 0; j < elem->variables->used; j++)
        printf("==> [debug] var[%s]\n", (char*) elem->variables->values[j]);
    }
    for (uint32_t j = 0; j < elem->variables->used; j++)
    {
      if (makeit_init_value(project, (char**) &elem->variables->values[j], directory) != 1)
        return 0;
    }
    if (makeit_process_functions(project, elem->name, elem->variables, directory) != 1)
      return 0;
  }
  return 1;
}

int makeit_parse_file(makeit_project* project, const char* filepath)
{
  const char* directory = strdir(filepath);
  uint32_t length;
  uint8_t* data = file_utils_read(filepath, &length);
  return makeit_parse_data(project, (char*) data, directory);
}

static const char VAR_SEPARATOR = ' ';
int makeit_init_value(makeit_project* project, char** str, const char* directory)
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
