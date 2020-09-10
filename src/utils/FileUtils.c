#include "FileUtils.h"

#include "String.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

uint8_t* fsurd(const char* filepath, uint32_t* length)
{
  FILE* file = fopen(filepath, "rb");
  fseek(file, 0L, SEEK_END);
  *length = ftell(file);
  uint8_t* buffer = malloc(*length);
  rewind(file);
  fread(buffer, 1, *length, file);
  fclose(file);
  return buffer;
}

int fsuwr(const char* filepath, uint8_t* data, uint32_t length)
{
  FILE* file = fopen(filepath, "wb");
  if (file == NULL)
    return 0;

  fwrite(data, 1, length, file);
  fclose(file);
  return 1;
}

int fsumkd(const char* directory)
{
  mkdir(directory, 0700);
  return 1; // TODO:
}

bool fsuexist(const char* filepath)
{
  return access(filepath, R_OK) == 0;
}

uint32_t fsufind(const char* directory, const char* pattern, array* arr, bool sub_dirs)
{
  char* fixed_dir = strjoin(strpathfix(directory), "/");
  DIR* dir = opendir(fixed_dir);
  uint32_t n = 0;
  if (dir != NULL)
  {
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
      if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
      {
        char* filepath = (char*) malloc(strlen(fixed_dir) + strlen(entry->d_name) + 1);
        strcpy(filepath, fixed_dir);
        strcat(filepath, entry->d_name);
        if (sub_dirs && entry->d_type == 4)
          fsufind(filepath, pattern, arr, sub_dirs);
        else if (entry->d_type == 8)
        {
          if (strwcpmt(pattern, filepath))
          {
            array_push(arr, filepath);
            n++;
          }
        }
      }
    }
    closedir(dir);
  }
  return n;
}
