#include "MakeFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <me/mestr.h>
#include <me/mefil.h>

int MI_makefile(char* filepath, struct array* flags, struct array* sources, struct array* headers, struct makeit_project* project)
{
  /* create build directory */
  if (!me_file_exists(project->build_dir))
    me_file_mkdir(project->build_dir);

  struct string* source = mestr_new(1024);

  /* append name */
  mestr_apnd(source, "NAME = ");
  mestr_apnd(source, project->name);
  mestr_apndc(source, '\n');

  /* append build directory */
  mestr_apnd(source, "BUILD = ");
  mestr_apnd(source, project->build_dir->path);
  mestr_apndc(source, '\n');

  /* append flags */
  mestr_apnd(source, "FLAGS = ");
  for (uint32_t i = 0; i < flags->size; i++)
  {
    mestr_apnd(source, flags->entries[i]);
    if (i < flags->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  /* append compiler */
  mestr_apnd(source, "CC = ");
  mestr_apnd(source, project->compiler);
  mestr_apndc(source, '\n');

  mestr_apndc(source, '\n');


  /* append libraries */
  mestr_apnd(source, "LIBS = ");
  for (uint32_t i = 0; i < project->libs->size; i++)
  {
    mestr_apnd(source, "-l");
    mestr_apnd(source, project->libs->entries[i]);
    if (i < project->libs->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  /* append includes */
  mestr_apnd(source, "INCS = ");
  for (uint32_t i = 0; i < project->incs->size; i++)
  {
    mestr_apnd(source, "-i");
    mestr_apnd(source, project->incs->entries[i]);
    if (i < project->incs->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  /* append library directories */
  mestr_apnd(source, "LDIR = ");
  for (uint32_t i = 0; i < project->ldirs->size; i++)
  {
    mestr_apnd(source, "-L");
    mestr_apnd(source, project->ldirs->entries[i]);
    if (i < project->ldirs->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  /* append include directories */
  mestr_apnd(source, "IDIR = ");
  for (uint32_t i = 0; i < project->idirs->size; i++)
  {
    mestr_apnd(source, "-I");
    mestr_apnd(source, project->idirs->entries[i]);
    if (i < project->idirs->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  /* append definitions */
  mestr_apnd(source, "DEFS = ");
  for (uint32_t i = 0; i < project->definitions->size; i++)
  {
    mestr_apnd(source, "-D");
    mestr_apnd(source, project->definitions->entries[i]);
    if (i < project->definitions->size - 1)
      mestr_apndc(source, ' ');
  }
  mestr_apndc(source, '\n');

  mestr_apndc(source, '\n');

  /* sources */
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

  /* headers */
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

  /* objects */
  mestr_apnd(source, "# <-- objects -->\n");
  mestr_apnd(source, "OBJS = ");
  for (uint32_t i = 0; i < sources->size; i++)
  {
    struct file* obj_file = me_file_join(project->build_dir->path, me_file_chext(sources->entries[i], "o"));
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

  /* clean */
  mestr_apnd(source, ".PHONY: clean\n\n");
  mestr_apnd(source, "clean:\n");
  mestr_apnd(source, "\trm -f $(OBJS)\n");

  me_file_write(filepath, mestr_extrd(source), source->length);
  return 1;
}
