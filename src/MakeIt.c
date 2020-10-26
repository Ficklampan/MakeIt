#include "MakeIt.h"

#include "utils/Time.h"

#include <me/mestr.h>
#include <me/mefil.h>

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "Config.h"

#include "Texts.h"
#include "MakeItFunctions.h"

#include "script/lexer/MakeItLexer.h"
#include "script/parser/MakeItParser.h"

static struct option long_options[] = {
  {"help", no_argument, 0, 'h'},
  {"version", no_argument, 0, 'v'},
  {"debug", no_argument, 0, 'd'},
  {"functions", no_argument, 0, 'f'},
  {"info", required_argument, 0, 'i'}
};

struct makeit_project project;

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

  /* default filepath */
  char* filepath = "init.makeit";

  /* if a filepath was specified then use that */
  if (optind < argc)
    filepath = argv[optind];

  struct file* file = me_file_new(filepath);

  if (!me_file_exists(file))
  {
    printf(":: file not found `%s`.\n", filepath);
    return 1;
  }

  /* create script */
  struct mi_script* script = MI_script_new(memap_new(32), memap_new(32));
  /* add functions */
  memap_push(script->functions, "print", MI_function_new(MIFUNC_print));
  memap_push(script->functions, "system", MI_function_new(MIFUNC_system));
  memap_push(script->functions, "project", MI_function_new(MIFUNC_project));
  memap_push(script->functions, "version", MI_function_new(MIFUNC_version));
  memap_push(script->functions, "language", MI_function_new(MIFUNC_language));
  memap_push(script->functions, "build_dir", MI_function_new(MIFUNC_build_dir));
  memap_push(script->functions, "compiler", MI_function_new(MIFUNC_compiler));
  memap_push(script->functions, "makefile", MI_function_new(MIFUNC_makefile));
  memap_push(script->functions, "script", MI_function_new(MIFUNC_script));
  memap_push(script->functions, "define", MI_function_new(MIFUNC_define));
  memap_push(script->functions, "library", MI_function_new(MIFUNC_library));
  memap_push(script->functions, "include", MI_function_new(MIFUNC_include));
  memap_push(script->functions, "library_dir", MI_function_new(MIFUNC_library_dir));
  memap_push(script->functions, "include_dir", MI_function_new(MIFUNC_include_dir));

  /* create project */
  project.name = "";
  project.version = "";
  project.language = mearr_new(8);

  project.build_dir = me_file_new("build");
  project.compiler = "";

  project.configs = mearr_new(8);
  project.libs = mearr_new(8);
  project.incs = mearr_new(8);
  project.ldirs = mearr_new(8);
  project.idirs = mearr_new(8);
  project.definitions = mearr_new(8);

  /* process file */
  if (MI_procfile(file, script) != 1)
  {
    printf("\e[31m:: errors occurred while parsing file `%s`.\e[0m\n", filepath);
    return 1;
  }
  uint64_t end_millis = tmillis();
  printf("==> MakeIt made it without errors");
  if (CFG_millis())
    printf(", in %lu milliseconds!\n", (end_millis - start_millis));
  else
    printf("!\n");
  return 0;
}

int MI_procdat(struct file* file, void* data, uint32_t size, struct mi_script* script)
{
  struct mi_lexer* lexer = MI_lexer_new(mearr_new(128));
  struct mi_parser* parser = MI_parser_new(file, script);

  if (MI_make_tokens(lexer, (char*) data, size) != 1)
    return 0;

  if (MI_parse_tokens(parser, lexer->tokens) != 1)
    return 0;

  return 1;
}

int MI_procfile(struct file* file, struct mi_script* script)
{
  if (CFG_debug())
    printf("==> [debug] file[%s]\n", file->path);

  void* data;
  uint32_t size;
  if (me_file_read(file->path, &data, &size) != FILE_SUCCESS)
  {
    printf(":: failed to read file '%s'.\n", file->path);
    return 0;
  }
  return MI_procdat(file, data, size, script);
}
