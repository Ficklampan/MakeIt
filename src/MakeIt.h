#ifndef MAKEIT_H
  #define MAKEIT_H

typedef struct {
  char* name;
  map vars;
  char* directory;
} makeit_project;

int makeit_parse_data(makeit_project* project, const char* data);
int makeit_parse_file(makeit_project* project, const char* filepath);
int makeit_process_function(makeit_project* project, const char* func, const char** args, uint32_t argc);
int makeit_init_values(makeit_project* project, char** args, uint32_t argc);
int makeit_init_value(makeit_project* project, char** str);

#endif
