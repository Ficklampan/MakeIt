#ifndef MAKEIT_H
  #define MAKEIT_H

#include "utils/Map.h"

#define NDEBUG

typedef struct {
  char* name;
  map* vars;
  array* include_paths;
  array* lib_paths;
  array* definitions;
  char* directory;
  char* filepath;
} makeit_project;

int makeit_init_project(makeit_project* project, char* name);
int makeit_parse_data(makeit_project* project, const char* data, const char* directory);
int makeit_parse_file(makeit_project* project, const char* filepath);
int makeit_init_value(makeit_project* project, char** str, const char* directory);

#endif
