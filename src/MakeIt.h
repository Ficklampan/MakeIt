#ifndef MAKEIT_H
  #define MAKEIT_H

#include "utils/List.h"
#include "utils/String.h"

typedef struct {
  string name;
  list sources;
  list headers;
  string directory;
} makeit_project;

int makeit_parse_data(makeit_project* project, const string data);
int makeit_parse_file(makeit_project* project, const char* filepath);
int makeit_process_function(makeit_project* project, const string func, const string* args, uint32_t argc);
int makeit_init_values(makeit_project* project, string* args, uint32_t argc);
int makeit_init_value(makeit_project* project, string str);

#endif
