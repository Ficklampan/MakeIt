#include "MakeIt.hpp"

#include "utils/Time.hpp"

#include "functions/Functions.hpp"

#include "script/Script.hpp"

#include <getopt.h>
#include <memory>

struct Config {
  std::string file;
  bool print_time;
  bool print_memory;
} config = {
  .file = "MIBuild",
  .print_time = false,
  .print_memory = false
};

void usage()
{
  printf("Usage: makeit [flags] [directory/file]\n");
  printf("Flags:\n");
  printf("  -h, --help                   Prints this message and exit\n");
  printf("  -v, --version                Prints the current version\n");
  printf("  -t, --time                   Print the execution time\n");
  printf("  -m, --memory                 Print memory usage\n");
  printf("  -f, --file                   Script file\n");
  printf("Report bugs at <https://github.com/Ficklampan/MakeIt/issues>\n");
}

void version()
{
  printf("Version: %s\n", "something");
  printf("Compiler: %s\n", __VERSION__);
}

int parseArgs(int argc, char** argv)
{
  static struct option long_options[] = {
    {"help", optional_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {"time", no_argument, 0, 't'},
    {"memory", no_argument, 0, 'm'},
    {"file", required_argument, 0, 'f'},
  };

  int index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "h::vf:tm", long_options, &index)) != -1)
  {
    switch (c)
    {
      /* help */
      case 'h':
	if (optarg != nullptr)
	  MI::function::usage(optarg);
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

      /* memory */
      case 'm':
	config.print_memory = true;
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

int init()
{
  /* init */
  uint64_t start_millis = tmillis();

  me::File file(config.file);
  if (!file.exists())
  {
    printf(":: file not found `%s`.\n", config.file.c_str());
    return 1;
  }

  std::shared_ptr<MI::Storage> storage = std::make_shared<MI::Storage>();
  storage->functions["print"] = MI::function::getPrint();
  storage->functions["system"] = MI::function::getSystem();
  storage->functions["search"] = MI::function::getSearch();

  storage->functions["project"] = MI::function::getProject();
  storage->functions["source"] = MI::function::getSource();
  storage->functions["makefile"] = MI::function::getMakefile();
  storage->functions["ycm"] = MI::function::getYCM();

  uint64_t init_millis = tmillis();

  /* process file */
  if (!MI::readScript(&file, storage.get()))
    return 0;

  uint64_t end_millis = tmillis();

  double init_time = (double) (init_millis - start_millis) / 1000.0;
  double script_time = (double) (end_millis - init_millis) / 1000.0;
  double total_time = (double) (end_millis - start_millis) / 1000.0;

  if (config.print_time)
    printf("Time: [Initializing %.3fms, Parsing %.3fms, Total %.3fms]\n", init_time, script_time, total_time);

  return 1;
}

int main(int argc, char** argv)
{
  if (parseArgs(argc, argv))
    return 0;

  if (!init())
    return 1;

  return 0;
}
