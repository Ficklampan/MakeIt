#include "MakeIt.hpp"

#include "utils/Time.h"

#include <getopt.h>

#include "Texts.h"
#include "Functions.hpp"

#include "script/lexer/Lexer.hpp"
#include "script/parser/Parser.hpp"
#include "script/vm/MiVm.hpp"

static struct option long_options[] = {
  {"help", no_argument, 0, 'h'},
  {"version", no_argument, 0, 'v'},
  {"file", required_argument, 0, 'f'},
  {"info", required_argument, 0, 'i'}
};

void pusage()
{
  printf("Usage: makeit [flags] [directory/file]\n");
  printf("Flags:\n");
  printf("  -h, --help                   Prints this message and exit\n");
  printf("  -v, --version                Prints the current version\n");
  printf("  -d, --debug                  Prints this message and exit\n");
  printf("  -f, --file                   Script file\n");
  printf("  -i FUNC, --info=FUNC         Prints info about a function\n\n");
  printf("Report bugs at <https://github.com/Ficklampan/MakeIt/issues>\n");
}

void pversion()
{
  printf("Version: %s\n", "something");
  printf("GNU GCC: %s\n", __VERSION__);
}

int main(int argc, char** argv)
{
  /* default file path */
  std::string filepath = "init.makeit";
  std::string config;

  /* get options */
  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "hfi:", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 'h':
        pusage();
        return 0;
      case 'v':
      	pversion();
	return 0;
      case 'f':
	filepath = optarg;
      case 'i':
	mi::function::usage(optarg);
        return 0;
      case '?':
        printf(":: unknown option '%c'.\n", (char) c);
        return 0;
      default:
        abort();
    }
  }

  /* init */
  uint64_t start_millis = tmillis();

  if (optind < argc)
    config = argv[optind];

  me::File* file = new me::File(filepath);
  if (!file->exists())
  {
    printf(":: file not found `%s`.\n", filepath.c_str());
    return 1;
  }

  mi::Project* project = new mi::Project();

  project->storage.functions["print"] = new mi::Function(mi::function::print, {0, 0});
  project->storage.functions["system"] = new mi::Function(mi::function::system, {0});
  project->storage.functions["search"] = new mi::Function(mi::function::search, {0});

  project->storage.functions["project"] = new mi::Function(mi::function::project, {0});
  project->storage.functions["script"] = new mi::Function(mi::function::script, {0});

  project->storage.functions["define"] = new mi::Function(mi::function::define, {0});
  project->storage.functions["library"] = new mi::Function(mi::function::library, {0});
  project->storage.functions["include"] = new mi::Function(mi::function::include, {0});
  project->storage.functions["library_dir"] = new mi::Function(mi::function::library_dir, {0});
  project->storage.functions["include_dir"] = new mi::Function(mi::function::include_dir, {0});

  project->storage.functions["make_config"] = new mi::Function(mi::function::make_config, {0});
  project->storage.functions["ycm_config"] = new mi::Function(mi::function::ycm_config, {0});

  project->config.name = "noname";
  project->config.version = "1.0.0";
  project->config.language = "c";
  project->config.compiler = "gcc";
  project->config.build_dir = new me::File("./build");

  /* process file */
  if (!mi::readScript(file, project))
  {
    printf("\e[31m:: errors occurred while parsing file `%s`.\e[0m\n", filepath.c_str());
    return 1;
  }

  uint64_t end_millis = tmillis();
  double time = (double) (end_millis - start_millis) / 1000.0;
  printf("==> Completed in %.3f milliseconds.\n", time);

  return 0;
}

int mi::readScript(me::File* file, const std::string &source, Project* project)
{
  printf("lex\n");
  Lexer lexer(source);
  if (!lexer.makeTokens())
    return 0;

  printf("parse\n");
  Parser parser(new me::Iterator<Token*>(lexer.getTokens().data(), lexer.getTokens().size()));
  if (!parser.parseTokens())
    return 0;

  printf("vm\n");
  VM vm(&project->storage, &parser.getInstructions());
  while (vm.clock());

  return 1;
}

int mi::readScript(me::File* file, Project* project)
{
  void* data;
  uint32_t size;

  if (!file->read(data, size))
  {
    printf(":: failed to read file '%s'.\n", file->getPath().c_str());
    return 0;
  }

  std::string source((char*) data, size);

  return readScript(file, source, project);
}
