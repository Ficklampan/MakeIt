#include "GNUMake.hpp"

#include "../System.hpp"

#define FOR_EACH(v, e) { \
  for (uint32_t i = 0; i < v.size(); i++) \
  { \
    bool last = i == v.size() - 1; \
    e \
  } \
}

makeit::GNUMake::GNUMake(const std::string &name, const BuildConfig* config, const Options &options) : name(name), config(config), options(options)
{
}

int makeit::GNUMake::write_header(std::string &str)
{
  str.append("NAME = ").append(name) += '\n';
  str.append("CC = ").append(config->cc) += '\n';
  str.append("BUILD = ").append(config->build.getPath()) += '\n';
  str += '\n';

  str.append("FLAGS = ");
  FOR_EACH(config->flags, { str.append(*config->flags.at(i)); if (!last) str += ' '; });
  str += '\n';
  str += '\n';

  str.append("LIBS = ");
  FOR_EACH(config->libraries, { str.append("-l").append(*config->libraries.at(i)); if (!last) str += ' '; });
  str += '\n';
  str.append("INCS = ");
  FOR_EACH(config->includes, { str.append("-i").append(*config->includes.at(i)); if (!last) str += ' '; });
  str += '\n';
  str.append("LPATHS = ");
  FOR_EACH(config->library_paths, { str.append("-L").append(*config->library_paths.at(i)); if (!last) str += ' '; });
  str += '\n';
  str.append("IPATHS = ");
  FOR_EACH(config->include_paths, { str.append("-I").append(*config->include_paths.at(i)); if (!last) str += ' '; });
  str += '\n';
  str += '\n';

  str.append("DEFS = ");
  uint32_t index = 0;
  for (auto &[key, value] : config->definitions)
  {
    str.append("-D").append(key);

    if (value != nullptr)
      str.append("=").append(*value);

    if (index < config->definitions.size() - 1)
      str += ' ';

    index++;
  }
  str += '\n';
  str += '\n';

  if (options.depends.size() > 0)
  {
    str.append("EXTRADEPS = ");
    FOR_EACH(options.depends, { str.append(*options.depends.at(i)); if (!last) str += ' '; });

    str += '\n';
    str += '\n';
  }

  str.append("COPTS = $(FLAGS) $(INCS) $(IPATHS) $(DEFS)\n");
  str.append("LOPTS = $(FLAGS) $(LIBS) $(LPATHS)\n\n");
  return 1;
}

int makeit::GNUMake::write_depends(std::string &str)
{
  str.append("SOURCES = ");
  FOR_EACH(config->sources, { str.append(*config->sources.at(i)); if (!last) str.append(" \\\n\t"); });
  str += '\n';
  str += '\n';

  str.append("OBJECTS = $(SOURCES:%=$(BUILD)/%.o)\n");
  str.append("DEPENDS = $(OBJECTS:%.o=%.d)\n\n");

  str.append("$(NAME): $(OBJECTS)").append(options.depends.size() > 0 ? " $(EXTRADEPS)\n" : "\n");
  str.append("\t@$(CC) -o $@ $^ $(LOPTS)\n\n");

  str.append("-include $(DEPENDS)\n\n");

  str.append("$(BUILD)/%.o: %\n");
  str.append("\t@echo \"==> \e[32mcompiling source \e[33m[$<]\e[0m\"\n");
  str.append("\t@mkdir -p $(@D)\n");
  str.append("\t@$(CC) -c -o $@ $< $(COPTS) -MMD\n\n");

  return 1;
}

int makeit::GNUMake::write_footer(std::string &str)
{
  str.append(".PHONY: clean\n\n");
  str.append("clean:\n\trm -f $(OBJECTS) $(DEPENDS)\n");
  return 1;
}

int makeit::GNUMake::generate(std::string &str)
{
  if (!write_header(str))
    return 0;
  if (!write_depends(str))
    return 0;
  if (!write_footer(str))
    return 0;
  return 1;
}
