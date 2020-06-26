#include "MakeIt.h"
#include "utils/FileUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int makeit_parse_data(makeit_project* project, const string data)
{
  char* str_buffer = malloc(data.length);
  uint32_t index = 0;
  char in_str = 0;
  for (uint32_t i = 0; i < data.length; i++)
  {
    char c = data.ptr[i];
    char last = i > 0 ? data.ptr[i - 1] : 0;
    if (c == '"' && last != '\\')
    {
      str_buffer[index++] = '"';
      in_str = in_str == 0 ? 1 : 0;
      continue;
    }
    if (in_str == 1)
    {
      str_buffer[index++] = c;
      continue;
    }
    if (c == ' ' || c == '\n')
      continue;
    else if (last == ' ' || last == '\n')
      str_buffer[index++] = ';';
    str_buffer[index++] = c;
  }
  str_buffer[index] = 0;
  string fixed = {index, str_buffer};
  uint32_t argc;
  string* args = string_split(&fixed, ';', &argc);
  for (uint32_t i = 0; i < argc; i++)
  {
    uint32_t count;
    string* func = string_split(&args[i], ':', &count);
    uint32_t func_argc;
    string* func_args = string_split(&func[1], ',', &func_argc);
    if (count > 1 && makeit_process_function(project, func[0], func_args, func_argc) != 1)
      return 0;
  }
  return 1;
}

int makeit_parse_file(makeit_project* project, const char* filepath)
{
  uint32_t length;
  uint8_t* data = file_utils_read(filepath, &length);
  string str = {length, (char*) data};
  return makeit_parse_data(NULL, str);
}

int makeit_process_function(makeit_project* project, const string func, const string* args, uint32_t argc)
{
  printf("function: %s\n", func.ptr);
  printf("  has:\n");
  for (uint32_t i = 0; i < argc; i++)
    printf("    %s\n", args[i].ptr);
  return 1;
}
