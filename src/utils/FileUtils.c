#include "FileUtils.h"

#include "String.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

uint8_t* file_utils_read(const char* filepath, uint32_t* length)
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
void file_utils_write(const char* filepath, uint8_t* data, uint32_t length)
{
  FILE* file = fopen(filepath, "wb");
  fwrite(data, 1, length, file);
  fclose(file);
}

void file_utils_mkdir(const char* directory)
{
  mkdir(directory, 0700);
}

bool file_utils_exists(const char* filepath)
{
  return access(filepath, R_OK) == 0;
}

uint32_t file_utils_find(const char* directory, const char* pattern, array* arr, bool sub_dirs)
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
          file_utils_find(filepath, pattern, arr, sub_dirs);
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
