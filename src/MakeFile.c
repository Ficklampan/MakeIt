#include "MakeFile.h"

#include "utils/String.h"
#include "utils/FileUtils.h"

#include <stdio.h>
#include <stdlib.h>

int make_makefile(char* name, char* directory, char* filepath, char* flags, char* sources, char* headers, char* libs, array* include_paths, array* lib_paths, array* definitions, char* build_path, char** info)
{
  array* sources_arr = strsplit(sources, ' ');
  array* libs_arr = strsplit(libs, ' ');

  string_buffer* source = calloc(sizeof(string_buffer), 1);
  string_buffer_init(source, 1024);

  string_buffer_append(source, "NAME = ");
  string_buffer_append(source, name);
  string_buffer_append(source, "\n");

  /* appending sources */
  string_buffer_append(source, "SRC = ");
  string_buffer_append(source, sources);
  string_buffer_append(source, "\n");

  /* appending headers */
  string_buffer_append(source, "HEADERS = ");
  string_buffer_append(source, headers);
  string_buffer_append(source, "\n");

  string_buffer_append(source, "OBJ = ");
  for (uint32_t i = 0; i < sources_arr->used; i++)
  {
    if (strempty(sources_arr->values[i]))
      continue;

    /* building the object path */
    string_buffer* path = (string_buffer*) calloc(sizeof(string_buffer), 1);
    string_buffer_init(path, 128);
    string_buffer_append(path, directory);
    string_buffer_append(path, build_path);
    string_buffer_appendc(path, '/');
    char* filext = strfilext(sources_arr->values[i], "o");
    string_buffer_append(path, strsub(filext, strlen(directory), strlen(filext)));

    string_buffer_append(source, path->str);
    string_buffer_appendc(source, ' ');
    //boost::filesystem::create_directories(string_utils::directory(path));
  }

  string_buffer_append(source, "\n");

  string_buffer_append(source, "CC = g++\n");
  string_buffer_append(source, "CFLAGS = ");
  string_buffer_append(source, flags);
  string_buffer_append(source, "\n");

  string_buffer_append(source, "OUTD = ");
  string_buffer_append(source, build_path);
  string_buffer_append(source, "\n");

  string_buffer_append(source, "LIBD = ");
  for (uint32_t i = 0; i < lib_paths->used; i++)
  {
    if (strempty(lib_paths->values[i]))
      continue;

    string_buffer_append(source, "-L");
    string_buffer_append(source, lib_paths->values[i]);
    string_buffer_appendc(source, ' ');
  }
  string_buffer_append(source, "\n");

  string_buffer_append(source, "INCD = ");
  for (uint32_t i = 0; i < include_paths->used; i++)
  {
    if (strempty(include_paths->values[i]))
      continue;

    string_buffer_append(source, "-I");
    string_buffer_append(source, include_paths->values[i]);
    string_buffer_appendc(source, ' ');
  }
  string_buffer_append(source, "\n");

  string_buffer_append(source, "DEFS = ");
  for (uint32_t i = 0; i < definitions->used; i++)
  {
    if (strempty(definitions->values[i]))
      continue;

    string_buffer_append(source, "-D");
    string_buffer_append(source, definitions->values[i]);
    string_buffer_appendc(source, ' ');
  }
  string_buffer_append(source, "\n");

  string_buffer_append(source, "LIBS = ");
  for (uint32_t i = 0; i < libs_arr->used; i++)
  {
    if (strempty(libs_arr->values[i]))
      continue;

    string_buffer_append(source, "-l");
    string_buffer_append(source, libs_arr->values[i]);
    string_buffer_appendc(source, ' ');
  }
  string_buffer_append(source, "\n");

  /* compile sources */
  string_buffer_append(source, "$(OUTD)/%.o: %.cpp $(HEADERS)\n");
  string_buffer_append(source, "\t$(CC) -c -o $@ $< $(CFLAGS) $(INCD) $(DEFS)\n");

  /* link */
  string_buffer_append(source, "$(NAME): $(OBJ)\n");
  string_buffer_append(source, "\t$(CC) -o $@ $^ $(LIBD) $(LIBS)\n");

  /* cleanup */
  string_buffer_append(source, ".PHONY: clean\n");
  string_buffer_append(source, "clean:\n");
  string_buffer_append(source, "\trm -f $(OBJ)\n");

  file_utils_write(filepath, (uint8_t*) source->str, source->length);
  return 1;
}
