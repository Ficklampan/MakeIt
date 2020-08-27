#ifndef MAKEIT_H
  #define MAKEIT_H

#define MI_VERSION		"2020-08-18"

#include "utils/Arrays.h"

enum lang_t {
  LANG_C, LANG_CPP
};

typedef struct {
  char* name;
  char* version;
  enum lang_t lang;
  map* vars;
  array* libs;
  array* incs;
  array* ldirs;
  array* idirs;
  array* definitions;

  char* directory;
  char* filepath;
  char* deps_directory;
} makeit_project;

int MI_initproj(makeit_project* project, char* name, char* version, char* lang);
int MI_procdat(makeit_project* project, const char* data, uint32_t data_length, const char* filepath, const char* directory);
int MI_procfile(makeit_project* project, const char* filepath);
int MI_procval(makeit_project* project, char** str, const char* directory);

#endif
