#include "MakeIt.hpp"

#include "utils/Time.hpp"

#include <getopt.h>

#include "Functions.hpp"

#include "script/Lexer.hpp"
#include "script/Parser.hpp"


void usage()
{
  printf("Usage: makeit [flags] [directory/file]\n");
  printf("Flags:\n");
  printf("  -h, --help                   Prints this message and exit\n");
  printf("  -v, --version                Prints the current version\n");
  printf("  -d, --debug                  Prints this message and exit\n");
  printf("  -f, --file                   Script file\n");
  printf("Report bugs at <https://github.com/Ficklampan/MakeIt/issues>\n");
}

void version()
{
  printf("Version: %s\n", "something");
  printf("GNU GCC: %s\n", __VERSION__);
}

int MI::parseArgs(int argc, char** argv)
{
  static struct option long_options[] = {
    {"help", optional_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {"time", no_argument, 0, 't'},
    {"file", required_argument, 0, 'f'},
  };

  int index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "h::vf:t", long_options, &index)) != -1)
  {
    switch (c)
    {
      /* help */
      case 'h':
	if (optarg != nullptr)
	  function::usage(optarg);
	else
	  usage();
	return 1;

      /* version */
      case 'v':
      	version();
	return 1;

      /* time */
      case 't':
	config.print_time = true;
	break;

      /* file */
      case 'f':
	config.file = optarg;
	break;

      case '?':
      default:
        abort();
    }
  }
  return 0;
}

void MI::initProject(Project &project)
{
  project.name = "noname";
  project.version = "1.0.0";
  project.language = "c";
  project.compiler = "gcc";
  project.build_dir = new me::File("./build");
}

void MI::initStorage(Storage &storage, Project* project)
{
  storage.functions["print"] = function::getFunction(nullptr, "print");
  storage.functions["system"] = function::getFunction(nullptr, "system");
  storage.functions["search"] = function::getFunction(nullptr, "search");

  storage.functions["project"] = function::getFunction(project, "project");
  storage.functions["makefile"] = function::getFunction(project, "makefile");
}

int MI::initScript()
{
  /* init */
  uint64_t start_millis = tmillis();

  me::File file(config.file);
  if (!file.exists())
  {
    printf(":: file not found `%s`.\n", config.file.c_str());
    return 1;
  }

  Project project;
  Storage storage;

  initProject(project);
  initStorage(storage, &project);

    /* process file */
  if (!readScript(file, storage))
    return 0;

  uint64_t end_millis = tmillis();
  double time = (double) (end_millis - start_millis) / 1000.0;

  if (config.print_time)
    printf("[time: %.3fms]\n", time);

  return 1;
}

int main(int argc, char** argv)
{
  if (MI::parseArgs(argc, argv))
    return 0;

  if (!MI::initScript())
    return 1;

  return 0;
}

int MI::readScript(me::File &file, Storage &storage)
{
  Lexer lexer;
  Parser parser;

  void* data;
  uint32_t size;

  if (!file.read(data, size))
    return 0;
  std::string source((char*) data, size);

  std::vector<Token*> tokens;

  if (!lexer.make(source, tokens))
    return 0;

  me::BasicIterator<Token*> token_iter(tokens.data(), tokens.size());
  if (!parser.parse(token_iter, storage))
    return 0;

  return 1;
}
