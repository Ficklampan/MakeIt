#include "MakeIt.hpp"
#include "Config.hpp"
#include "System.hpp"
#include "Common.hpp"

#include "functions/Functions.hpp"

#include "script/Script.hpp"

#include <getopt.h>
#include <locale.h>
#include <memory>
#include <cstring>

Config config = {
  .file = "MIBuild",
  .print_all = false,
  .block_all = false,
  .debug = false
};

struct Cache {
  std::map<std::string, std::string> vars;
} cache;

void makeit::usage(const char* arg)
{
  const char* FLAG_HELP_SHORT = _("Print usage");
  const char* FLAG_VERSION_SHORT = _("Print compiler and this version");
  const char* FLAG_PRINT_ALL_SHORT = _("Print changes on the system");
  const char* FLAG_BLOCK_ALL_SHORT = _("Confirm when making system changes");
  const char* FLAG_DEBUG_SHORT = _("Enable debug level L");
  const char* FLAG_FILE_SHORT = _("Specify file to be read");

  const char* FLAG_HELP_LONG = _("View info and stuff\nUse 'help FLAG' to show info and usage about FLAG");
  const char* FLAG_VERSION_LONG = _("View the compiler version the program was compiled in and the current software version");
  const char* FLAG_PRINT_ALL_LONG = _("If print all flag is specified then the software will print changes to the system (file being written/read or a command being executed)");
  const char* FLAG_BLOCK_ALL_LONG = _("If block all flag is specified then the software will ask before making changes to the system (file being written/read or a command being executed)");
  const char* FLAG_DEBUG_LONG = _("Higher level = more debuging");
  const char* FLAG_FILE_LONG = _("Change the file target. Default is 'MIBuild'");

  if (arg == nullptr)
  {
    printf("~~~| %s [WIP] |~~~\n", NAME);
    printf("Generates GNU 'Make' file\n");
    printf("Usage: makeit [flags]\n");
    printf("Flags:\n");
    printf("  -h flag, --help=flag         %s\n", FLAG_HELP_SHORT);
    printf("  -v, --version                %s\n", FLAG_VERSION_SHORT);
    printf("  -e, --print-all              %s\n", FLAG_PRINT_ALL_SHORT);
    printf("  -b, --block-all              %s\n", FLAG_BLOCK_ALL_SHORT);
    printf("  -d [L], --debug=[L]          %s\n", FLAG_DEBUG_SHORT);
    printf("  -f, --file                   %s\n", FLAG_FILE_SHORT);
    printf("Report issues at <https://github.com/Ficklampan/MakeIt/issues>\n");
  }else if (strcmp("help", arg) == 0)
  {
    printf("  %s --help: %s\n", NAME, FLAG_HELP_LONG);
  }else if (strcmp("version", arg) == 0)
  {
    printf("  %s --version: %s\n", NAME, FLAG_VERSION_LONG);
  }else if (strcmp("print-all", arg) == 0)
  {
    printf("  %s --print-all: %s\n", NAME, FLAG_PRINT_ALL_LONG);
  }else if (strcmp("block-all", arg) == 0)
  {
    printf("  %s --block-all: %s\n", NAME, FLAG_BLOCK_ALL_LONG);
  }else if (strcmp("debug", arg) == 0)
  {
    printf("  %s --debug: %s\n", NAME, FLAG_DEBUG_LONG);
  }else if (strcmp("file", arg) == 0)
  {
    printf("  %s --file: %s\n", NAME, FLAG_FILE_LONG);
  }else
  {
    printf("unknown flag '%s'\n", arg);
  }
}

void version()
{
  printf("Version: %s\n", "something");
  printf("Compiler: %s\n", __VERSION__);
}

void makeit::setupLocale()
{
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);
}

int makeit::parseArgs(int argc, char** argv)
{
  static struct option long_options[] = {
    {"help", optional_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {"print-all", no_argument, 0, 'e'},
    {"block-all", no_argument, 0, 'b'},
    {"debug", optional_argument, 0, 'd'},
    {"file", required_argument, 0, 'f'},
  };

  int index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "h::vf:ebd::", long_options, &index)) != -1)
  {
    switch (c)
    {
      /* help */
      case 'h':
	usage(optarg);
	return 1;

      /* version */
      case 'v':
      	version();
	return 1;

      /* print all */
      case 'e': config.print_all = true; break;
      /* block all */
      case 'b': config.block_all = true; break;

      /* debug */
      case 'd':
	if (optarg != nullptr)
	{
	  config.debug = std::stoi(optarg);
	  if (config.debug > 3)
	  {
	    printf("unknown debug level\n");
	    return 1;
	  }
	}else
	  config.debug = 1;
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

int makeit::init()
{
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
  storage->functions["library"] = MI::function::getLibrary();
  storage->functions["library_path"] = MI::function::getLibraryPath();
  storage->functions["include"] = MI::function::getInclude();
  storage->functions["include_path"] = MI::function::getIncludePath();
  storage->functions["define"] = MI::function::getDefine();
  storage->functions["source"] = MI::function::getSource();
  storage->functions["makefile"] = MI::function::getMakefile();
  storage->functions["ycm"] = MI::function::getYCM();

  /* process file */
  if (!MI::readScript(&file, storage.get()))
    return 0;

  return 1;
}

int makeit::main(int argc, char** argv)
{
  if (parseArgs(argc, argv))
    return 0;

  setupLocale();
  if (!init())
    return 1;

  return 0;
}

int main(int argc, char** argv)
{
  return makeit::main(argc, argv);
}


/* System.hpp */

int MI::readFile(const me::File &file, void* &data, uint32_t &size)
{
  bool execute = true;

  if (config.print_all)
    printf("\e[33m==> reading data from '%s'\e[0m\n", file.getPath().c_str());

  if (config.block_all)
    execute = yesno(":: do you want to read data from file[%s]?", file.getPath().c_str());

  if (execute)
    return me::File::read(file, data, size);
  return 1;
}

int MI::writeFile(const me::File &file, void* data, uint32_t size)
{
  bool execute = true;

  if (config.print_all)
    printf("\e[33m==> writing data[%uB] to '%s'\e[0m\n", size, file.getPath().c_str());

  if (config.block_all)
    execute = yesno(":: do you want to write data to file[%s]?", file.getPath().c_str());

  if (!config.block_all && (file.isAbsolutePath()))
    execute = noyes(":: do you want to write data to file[%s] outside working directory?");

  if (execute)
    return me::File::write(file, data, size);
  return 1;
}

int MI::system(const char* cmd)
{
  bool execute = true;

  if (config.print_all)
    printf("\e[33m==> executing command '%s'\n", cmd);

  if (config.block_all)
    execute = yesno(":: do you want to execute '%s'?", cmd);

  if (execute)  
    return ::system(cmd);
  return 1;
}
