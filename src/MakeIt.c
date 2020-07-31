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
  makeit_project* project = calloc(sizeof(makeit_project), 1);
  project->directory = filepath;
  if (makeit_parse_file(project, filepath) != 1)
  {
    printf(":: Failed to parse `%s`\n", filepath);
    return 1;
  }
  printf("==> Complete!\n");
  return 0;
}

int makeit_init_project(makeit_project* project, const char* name)
{
  project->name = name;
  project->vars = calloc(sizeof(map), 1);
  return 1;
}

int makeit_parse_data(makeit_project* project, const char* data)
{
  uint32_t data_length = strlen(data);

  /* function name string buffer */
  string_buffer* func = calloc(sizeof(string_buffer), 1);
  string_buffer_init(func, 512);

  /* element value string buffer */
  string_buffer* element = calloc(sizeof(string_buffer), 1);
  string_buffer_init(element, 512);

  /* array of elements in function */
  array* elements = calloc(sizeof(array), 1);
  array_init(elements, 4);

  /* current state */
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
      continue;
    }else if (data_type == 2 && c == '<' && last == '<')
    {
      data_type = 0;
      makeit_process_functions(project, func->str, elements);
      string_buffer_clear(func);
      array_clear(elements);
      continue;
    }
    else if (data_type == 2 && c == '\n')
    {
      if (element->length > 0)
      {
        if (!makeit_init_value(project, &element->str))
          return 0;
        array_push(elements, element->str);
        element = calloc(sizeof(string_buffer), 1);
        string_buffer_init(element, 512);
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
  return makeit_parse_data(project, (char*) data);
}

int makeit_init_value(makeit_project* project, char** str)
{
  if (project->vars == NULL)
    return 1;
  for (uint32_t i = 0; i < project->vars->used; i++)
  {
    char* element_key = project->vars->keys[i];
    string_buffer* element_value = (string_buffer*) project->vars->values[i];

    char* key = strjoin(strjoin("$(", element_key), ")\0");

    uint32_t length;
    strreplace(*str, key, element_value->str, &length);
    *str = strreplace(*str, key, element_value->str, &length);
  }
  return 1;
}
