#include "MakeIt.hpp"
#include "Config.hpp"
#include "System.hpp"
#include "Text.hpp"

#include "functions/Functions.hpp"
#include "script/Script.hpp"

#include "Common.hpp"

#include <getopt.h>
#include <locale.h>
#include <unistd.h>
#include <cstdarg>
#include <memory>
#include <cstring>

makeit::Config config = {
  .file = "MIBuild",
  .print_all = false,
  .block_all = false,
  .execute_output = false,
  .debug = false
};

void makeit::usage(const char* arg)
{
  const char* TEXT_DESC = _("Generates GNU 'Make' file");
  const char* TEXT_USAGE = _("Usage: makeit [flags] config ...");
  const char* TEXT_REPORT = _("Report issues at");
  const char* TEXT_UNKNOWN_FLAG = _("unknown flag");

  const char* FLAG_HELP_SHORT = _("Print usage");
  const char* FLAG_VERSION_SHORT = _("Print compiler and this version");
  const char* FLAG_PRINT_ALL_SHORT = _("Print changes on the system");
  const char* FLAG_BLOCK_ALL_SHORT = _("Confirm when making system changes");
  const char* FLAG_EXECUTE_SHORT = _("Execute output");
  const char* FLAG_DEBUG_SHORT = _("Enable debug level L");
  const char* FLAG_FILE_SHORT = _("Specify file to be read");

  const char* FLAG_HELP_LONG = _("View info and stuff\nUse 'help FLAG' to show info and usage about FLAG");
  const char* FLAG_VERSION_LONG = _("View the compiler version the program was compiled in and the current software version");
  const char* FLAG_PRINT_ALL_LONG = _("If print all flag is specified then the software will print changes to the system (file being written/read or a command being executed)");
  const char* FLAG_BLOCK_ALL_LONG = _("If block all flag is specified then the software will ask before making changes to the system (file being written/read or a command being executed)");
  const char* FLAG_EXECUTE_LONG = _("Execute the output script when generating a build script");
  const char* FLAG_DEBUG_LONG = _("Higher level = more debuging");
  const char* FLAG_FILE_LONG = _("Change the file target. Default is 'MIBuild'");

  if (arg == nullptr)
  {
    printf("~~~| %s [WIP] |~~~\n", NAME);
    printf("%s\n", TEXT_DESC);
    printf("%s\n", TEXT_USAGE);
    printf("Flags:\n");
    printf("  -h FLAG, --help=FLAG         %s\n", FLAG_HELP_SHORT);
    printf("  -v, --version                %s\n", FLAG_VERSION_SHORT);
    printf("  -p, --print-all              %s\n", FLAG_PRINT_ALL_SHORT);
    printf("  -b, --block-all              %s\n", FLAG_BLOCK_ALL_SHORT);
    printf("  -e, --execute                %s\n", FLAG_EXECUTE_SHORT);
    printf("  -d [L], --debug=[L]          %s\n", FLAG_DEBUG_SHORT);
    printf("  -f, --file                   %s\n", FLAG_FILE_SHORT);
    printf("%s <https://github.com/Ficklampan/MakeIt/issues>\n", TEXT_REPORT);
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
  }else if (strcmp("execute", arg) == 0)
  {
    printf("  %s --execute: %s\n", NAME, FLAG_EXECUTE_LONG);
  }else if (strcmp("debug", arg) == 0)
  {
    printf("  %s --debug: %s\n", NAME, FLAG_DEBUG_LONG);
  }else if (strcmp("file", arg) == 0)
  {
    printf("  %s --file: %s\n", NAME, FLAG_FILE_LONG);
  }else
  {
    error("%s '%s'", TEXT_UNKNOWN_FLAG, arg);
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
    {"print-all", no_argument, 0, 'p'},
    {"block-all", no_argument, 0, 'b'},
    {"execute", no_argument, 0, 'e'},
    {"debug", optional_argument, 0, 'd'},
    {"warn", required_argument, 0, 'W'},
  };

  int index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "h::vf:pbed::", long_options, &index)) != -1)
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
      case 'p': config.print_all = true; break;

      /* block all */
      case 'b': config.block_all = true; break;

      /* execute output */
      case 'e': config.execute_output = true; break;

      /* debug */
      case 'd':
	if (optarg != nullptr)
	{
	  config.debug = std::stoi(optarg);
	  if (config.debug > 3)
	  {
	    error("unknown debug level\n");
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

  for (uint32_t i = optind; i < argc; i++)
  {
    config.configs.push_back(argv[i]);
  }

  return 0;
}

int makeit::init()
{
  me::File file(config.file);
  if (!file.exists())
  {
    printf(_(":: file not found `%s`.\n"), config.file.c_str());
    return 1;
  }

  std::shared_ptr<makeit::Storage> storage = std::make_shared<makeit::Storage>();
  storage->functions["print"] = makeit::function::make_print();
  storage->functions["system"] = makeit::function::make_system();
  storage->functions["search"] = makeit::function::make_search();

  storage->functions["project"] = makeit::function::make_project();
  storage->functions["library"] = makeit::function::make_library();
  storage->functions["library_path"] = makeit::function::make_library_path();
  storage->functions["include"] = makeit::function::make_include();
  storage->functions["include_path"] = makeit::function::make_include_path();
  storage->functions["define"] = makeit::function::make_define();
  storage->functions["flags"] = makeit::function::make_flags();
  storage->functions["source"] = makeit::function::make_source();
  storage->functions["extern"] = makeit::function::make_extern();
  storage->functions["files"] = makeit::function::make_files();
  storage->functions["makefile"] = makeit::function::make_makefile();
  storage->functions["ycm"] = makeit::function::make_YCM();

  for (std::string &cfg : config.configs)
  {
    storage->variables[cfg] = new Variable(nullptr, Variable::INTEGER, new int(1));
  }

  /* process file */
  if (!makeit::read_script(&file, storage.get()))
    return 0;

  return 1;
}

int makeit::main(int argc, char** argv)
{
  if (geteuid() == 0)
  {
    if (!noyes(_(":: avoid running as root. continue?")))
      return 0;
  }

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

int makeit::readFile(const me::File &file, void* &data, uint32_t &size)
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

int makeit::writeFile(const me::File &file, void* data, uint32_t size)
{
  bool execute = true;

  if (config.print_all)
    printf("\e[33m==> writing data[%uB] to '%s'\e[0m\n", size, file.getPath().c_str());

  if (config.block_all)
    execute = yesno(_(":: do you want to write data to file[%s]?"), file.getPath().c_str());

  if (!config.block_all && (file.isAbsolutePath()))
    execute = noyes(_(":: do you want to write data to file[%s] outside working directory?"));

  if (execute)
    return me::File::write(file, data, size);
  return 1;
}

int makeit::system(const char* cmd)
{
  bool execute = true;

  if (config.print_all)
    printf("\e[33m==> executing command '%s'\n", cmd);

  if (config.block_all)
    execute = yesno(_(":: do you want to execute '%s'?"), cmd);

  if (execute)  
    return ::system(cmd);
  return 1;
}

void makeit::error(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  printf("\e[31m%s: ", _("ERROR"));
  vprintf(format, args);
  printf("\e[0m\n");
  va_end(args);
}

void makeit::warning(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  printf("\e[33m%s: ", _("WARN"));
  vprintf(format, args);
  printf("\n\e[0m");
  va_end(args);
}
