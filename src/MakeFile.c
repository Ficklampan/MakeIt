#include "MakeFile.h"

#include "utils/String.h"
#include "utils/FileUtils.h"

#include "Texts.h"

#include <stdio.h>
#include <stdlib.h>

int make_makefile(char* name, char* version, char* directory, char* filepath, char* flags,
  array* sources,
  array* headers,
  array* libs,
  array* incs,
  array* include_paths,
  array* lib_paths,
  array* definitions,
  char* lang,
  char* build_path, char** info)
{
  string_buffer* source = calloc(sizeof(string_buffer), 1);
  string_buffer_init(source, 1024);

  char* compiler = strcmp(lang, "c") == 0 ? "gcc" : (strcmp(lang, "c++") == 0 ? "g++" : "gcc");

  string_buffer_append(source, "NAME = ");
  string_buffer_append(source, name);
  string_buffer_appendc(source, '\n');
  string_buffer_append(source, "VERSION = ");
  string_buffer_append(source, version);
  string_buffer_appendc(source, '\n');
  string_buffer_append(source, "NAMEV = ");
  string_buffer_append(source, name);
  string_buffer_appendc(source, '-');
  string_buffer_append(source, version);
  string_buffer_appendc(source, '\n');
  string_buffer_appendc(source, '\n');

  /* sources */
  string_buffer_append(source, "SRC = ");
  for (uint32_t i = 0; sources != NULL && i < sources->used; i++)
  {
    string_buffer_append(source, sources->values[i]);
    if (i < sources->used - 1)
      string_buffer_appendc(source, ' ');
  }
  string_buffer_append(source, "\n");

  /* headers */
  string_buffer_append(source, "HEADERS = ");
  for (uint32_t i = 0; headers != NULL && i < headers->used; i++)
  {
    string_buffer_append(source, headers->values[i]);
    if (i < headers->used - 1)
      string_buffer_appendc(source, ' ');
  }
  string_buffer_append(source, "\n");

  /* objects */
  string_buffer_append(source, "OBJ = ");
  for (uint32_t i = 0; sources != NULL && i < sources->used; i++)
  {
    char* obj = strjoin(build_path, strfilext(strsub(sources->values[i], strlen(directory), strlen(sources->values[i]), NULL), "o"));
    char* obj_dir = strdir(obj);
    string_buffer_append(source, obj);
    if (i < sources->used - 1)
      string_buffer_appendc(source, ' ');
    fsumkd(obj_dir);
  }
  string_buffer_append(source, "\n\n");

  /* compiler */
  string_buffer_append(source, "CC = ");
  string_buffer_append(source, compiler);
  string_buffer_appendc(source, '\n');

  /* flags */
  string_buffer_append(source, "CFLAGS = ");
  string_buffer_append(source, flags);
  string_buffer_appendc(source, '\n');

  /* libraries */
  string_buffer_append(source, "LIBS = ");
  for (uint32_t i = 0; libs != NULL && i < libs->used; i++)
  {
    if (strlen(libs->values[i]) < 1)
      continue;
    if (((char*) libs->values[i])[0] == '#')
    {
      array* args = strsplit(libs->values[i] + 1, ' ');
      if (args->used == 0)
      {
        *info = "in 'MakeFile.c'[94]: 'args->used' returned zero.\n";
        return 0;
      }

      string_buffer_append(source, "$(pkg-config ");
      for (uint32_t j = 1; j < args->used; j++)
      {
        string_buffer_append(source, (char*) args->values[j]);
        string_buffer_appendc(source, ' ');
      }
      string_buffer_append(source, (char*) args->values[0]);
      string_buffer_appendc(source, ')');
    }else
    {
      string_buffer_append(source, "-l");
      string_buffer_append(source, libs->values[i]);
    }
    if (i < libs->used - 1)
      string_buffer_appendc(source, ' ');
  }
  string_buffer_appendc(source, '\n');
 
  /* includes */
  string_buffer_append(source, "INCS = ");
  for (uint32_t i = 0; incs != NULL && i < incs->used; i++)
  {
    if (strlen(incs->values[i]) < 1)
      continue;
    if (((char*) incs->values[i])[0] == '#')
    {
      array* args = strsplit(incs->values[i] + 1, ' ');
      if (args->used == 0)
      {
        *info = "in 'MakeFile.c'[127]: 'args->used' returned zero.\n";
        return 0;
      }

      string_buffer_append(source, "$(pkg-config ");
      for (uint32_t j = 1; j < args->used; j++)
      {
        string_buffer_append(source, (char*) args->values[j]);
        string_buffer_appendc(source, ' ');
      }
      string_buffer_append(source, (char*) args->values[0]);
      string_buffer_appendc(source, ')');
    }else
    {
      string_buffer_append(source, "-i");
      string_buffer_append(source, incs->values[i]);
    }
    if (i < incs->used - 1)
      string_buffer_appendc(source, ' ');
  }
  string_buffer_appendc(source, '\n');

  /* definitions */
  string_buffer_append(source, "DEFS = ");
  for (uint32_t i = 0; definitions != NULL && i < definitions->used; i++)
  {
    string_buffer_append(source, "-D");
    string_buffer_append(source, definitions->values[i]);
    if (i < definitions->used - 1)
      string_buffer_appendc(source, ' ');
  }
  string_buffer_appendc(source, '\n');

  /* build path */
  string_buffer_append(source, "BDIR = ");
  string_buffer_append(source, build_path);
  string_buffer_appendc(source, '\n');

  /* include directories */
  string_buffer_append(source, "IDIR = ");
  for (uint32_t i = 0; include_paths != NULL && i < include_paths->used; i++)
  {
    string_buffer_append(source, "-I");
    string_buffer_append(source, include_paths->values[i]);
    if (i < include_paths->used - 1)
      string_buffer_appendc(source, ' ');
  }
  string_buffer_appendc(source, '\n');

  /* library directories */
  string_buffer_append(source, "LDIR = ");
  for (uint32_t i = 0; lib_paths != NULL && i < lib_paths->used; i++)
  {
    string_buffer_append(source, "-L");
    string_buffer_append(source, lib_paths->values[i]);
    if (i < lib_paths->used - 1)
      string_buffer_appendc(source, ' ');
  }
  string_buffer_append(source, "\n\n");

  string_buffer_append(source, "$(BDIR)/%.o: %.* $(HEADERS)\n");
  string_buffer_append(source, "\t$(CC) -c -o $@ $< $(CFLAGS) $(IDIR) $(INCS) $(DEFS) && ");
  string_buffer_append(source, "echo \e[32m$<\e[0m\n\n");

  string_buffer_append(source, "$(NAMEV): $(OBJ)\n");
  string_buffer_append(source, "\t$(CC) -o $@ $^ $(LDIR) $(LIBS)\n\n");

  string_buffer_append(source, ".PHONY: clean\n\n");

  string_buffer_append(source, "clean:\n");
  string_buffer_append(source, "\trm -f $(OBJ)\n");

  fsuwr(filepath, (uint8_t*) source->str, source->length);
  return 1;
}
