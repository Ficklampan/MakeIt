#include "GNUMake.hpp"

int MI::GNUMake_config(const std::string &filepath, std::vector<std::string> &sources, std::vector<std::string> &headers, Project& project)
{
  return 1;
}

/*
int GNUMake_config(char* filepath, struct array* sources, struct array* headers, struct mi_config* config)
{
  if (!me_file_exists(config->build_dir))
    me_file_mkdir(config->build_dir);

  struct string* source = mestr_new(1024);

  mestr_apnd(source, "NAME = ");
  mestr_apnd(source, config->name);
  mestr_apndc(source, '\n');

  mestr_apnd(source, "BUILD = ");
  mestr_apnd(source, config->build_dir->path);
  mestr_apndc(source, '\n');

  mestr_apnd(source, "FLAGS = ");
  for (uint32_t i = 0; i < config->flags->size; i++)
  {
    mestr_apnd(source, config->flags->entries[i]);
    if (i < config->flags->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  mestr_apnd(source, "CC = ");
  mestr_apnd(source, config->compiler);
  mestr_apndc(source, '\n');

  mestr_apndc(source, '\n');


  mestr_apnd(source, "LIBS = ");
  for (uint32_t i = 0; i < config->libs->size; i++)
  {
    mestr_apnd(source, "-l");
    mestr_apnd(source, config->libs->entries[i]);
    if (i < config->libs->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  mestr_apnd(source, "INCS = ");
  for (uint32_t i = 0; i < config->incs->size; i++)
  {
    mestr_apnd(source, "-i");
    mestr_apnd(source, config->incs->entries[i]);
    if (i < config->incs->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  mestr_apnd(source, "LDIR = ");
  for (uint32_t i = 0; i < config->ldirs->size; i++)
  {
    mestr_apnd(source, "-L");
    mestr_apnd(source, config->ldirs->entries[i]);
    if (i < config->ldirs->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  mestr_apnd(source, "IDIR = ");
  for (uint32_t i = 0; i < config->idirs->size; i++)
  {
    mestr_apnd(source, "-I");
    mestr_apnd(source, config->idirs->entries[i]);
    if (i < config->idirs->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  mestr_apnd(source, "DEFS = ");
  for (uint32_t i = 0; i < config->definitions->size; i++)
  {
    mestr_apnd(source, "-D");
    mestr_apnd(source, config->definitions->entries[i]);
    if (i < config->definitions->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  mestr_apndc(source, '\n');

  mestr_apnd(source, "# <-- sources -->\n");
  mestr_apnd(source, "SRC = ");
  for (uint32_t i = 0; i < sources->size; i++)
  {
    mestr_apndc(source, '\t');
    mestr_apnd(source, sources->entries[i]);

    if (i < sources->size - 1)
      mestr_apnd(source, " \\\n");
  }

  mestr_apndc(source, '\n');
  mestr_apndc(source, '\n');

  mestr_apnd(source, "# <-- headers -->\n");
  mestr_apnd(source, "HEADERS = ");
  for (uint32_t i = 0; i < headers->size; i++)
  {
    mestr_apndc(source, '\t');
    mestr_apnd(source, headers->entries[i]);

    if (i < headers->size - 1)
      mestr_apnd(source, " \\\n");
  }

  mestr_apndc(source, '\n');
  mestr_apndc(source, '\n');

  mestr_apnd(source, "# <-- objects -->\n");
  mestr_apnd(source, "OBJS = ");
  for (uint32_t i = 0; i < sources->size; i++)
  {
    struct file* obj_file = me_file_join(config->build_dir->path, me_file_chext(sources->entries[i], "o"));
    struct file* obj_dir = me_file_new(me_file_directory_path(obj_file));

    if (!me_file_exists(obj_dir))
      me_file_mkdir(obj_dir);

    mestr_apndc(source, '\t');
    mestr_apnd(source, obj_file->path);

    if (i < sources->size - 1)
      mestr_apnd(source, " \\\n");
  }

  mestr_apndc(source, '\n');
  mestr_apndc(source, '\n');

  mestr_apnd(source, "$(BUILD)/%.o: %.* $(HEADERS)\n");
  mestr_apnd(source, "\t@$(CC) -c -o $@ $< $(FLAGS) $(IDIR) $(INCS) $(DEFS) && \\\n");
  mestr_apnd(source, "\techo \e[32mcompiling [$<]\e[0m\n\n");

  mestr_apnd(source, "$(NAME): $(OBJS)\n");
  mestr_apnd(source, "\t@$(CC) -o $@ $^ $(LDIR) $(LIBS)\n\n");

  mestr_apnd(source, ".PHONY: clean\n\n");
  mestr_apnd(source, "clean:\n");
  mestr_apnd(source, "\trm -f $(OBJS)\n");

  me_file_write(filepath, mestr_extrd(source), source->length);
  return 1;
}
*/
