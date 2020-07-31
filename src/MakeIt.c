#include "utils/FileUtils.h"
#include "utils/String.h"
#include "utils/Map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texts.h"
#include "MakeIt.h"
#include "MakeItFunc.h"

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    printf(":: no filepath specified\n");
    return 1;
  }
  printf("==> Initializing MakeIt...\n");
  const char* filepath = argv[1];
  printf("==> Parsing `%s`\n", filepath);
  if (makeit_parse_file(NULL, filepath) != 1)
  {
    printf(":: Failed to parse `%s`\n", filepath);
    return 1;
  }
  printf("==> Complete!\n");
  return 0;
}

int makeit_parse_data(makeit_project* project, const char* data)
{
  // the data must end with a ',' for it to work correct
  uint32_t data_length = strlen(data);

  string_buffer* func = calloc(sizeof(string_buffer), 1);
  string_buffer_init(func, 512);

  string_buffer* element = calloc(sizeof(string_buffer), 1);
  string_buffer_init(element, 512);

  array* elements = calloc(sizeof(array), 1);
  array_init(elements, 4);

  uint8_t data_type = 0;

  for (uint32_t i = 0; i < data_length; i++)
  {
    char c = data[i];
    char last = i > 0 ? data[i - 1] : 0;

    if (c == '>' && last == '>')
    {
      data_type = 1;
      continue;
    }else if (data_type == 1 && c == ':')
    {
      data_type = 2;
      printf("func: %s\n", func->str);
      string_buffer_clear(func);
      array_clear(elements);
      continue;
    }else if (data_type == 2 && c == '<' && last == '<')
    {
      data_type = 0;
      //for (uint32_t j = 0; j < elements->used; i++)
        //printf("  var[%i]: %s\n", j, (char*) elements->values[i]);
      continue;
    }
    else if (data_type == 2 && c == '\n')
    {
      if (element->length > 0)
      {
        printf("  var[%i]: %s\n", elements->used, element->str);
        element = calloc(sizeof(string_buffer), 1);
        string_buffer_init(element, 512);
        array_push(elements, element->str);
      }
      continue;
    }

    if (data_type == 1 && c != ' ' && c != '>' && c != '<')
      string_buffer_append(func, c);
    else if (data_type == 2 && (last == '\\' || c != ' ') && c != '>' && c != '<' && c != '\\')
      string_buffer_append(element, c);
  }
  return 1;
}

int makeit_parse_file(makeit_project* project, const char* filepath)
{
  uint32_t length;
  uint8_t* data = file_utils_read(filepath, &length);
  return makeit_parse_data(NULL, (char*) data);
}

int makeit_process_function(makeit_project* project, const char* func, const char** args, uint32_t argc)
{
  if (func == NULL)
  {
    printf(":: nullptr string\n");
    return 0;
  }
  printf("==> func %s has:\n", func);
  for (uint32_t i = 0; i < argc; i++)
    printf("  %s\n", args[i]);

  makeit_init_values(project, args, argc);
  makeit_process_functions(project, func, args, argc);
  return 1;
}

// functions that replaces variable pointers to variable
int makeit_init_values(makeit_project* project, char** args, uint32_t argc)
{
  for (uint32_t i = 0; i < argc; i++)
  {
    if (makeit_init_value(project, &args[i]) != 1)
      return 0;
  }
  return 1;
}

int makeit_init_value(makeit_project* project, char** str)
{
  for (uint32_t i = 0; i < project->vars.used; i++)
  {
    char* element_key = project->vars.keys[i];
    char* element_value = project->vars.values[i];

    char* key = "$(";
    key = strapnd(key, element_key);
    key = strapnd(key, ")");

    printf("the key: %s\n", key); // remove

    uint32_t length;
    strreplace(*str, key, element_value, &length);
    *str = strreplace(*str, key, element_value, &length);
  }
  return 1;
}
// -------------------------------------------------- //
