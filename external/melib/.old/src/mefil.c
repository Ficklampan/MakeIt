#include "../include/me/mefil.h" /* TODO: */

#include "../include/me/mestr.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

#include <fnmatch.h>

/*
struct file* me_file_child(struct file* file, char* name)
{
  char* dir = me_file_directory_path(file);
  struct string* path = mestr_new(PATH_MAX);
  mestr_apnd(path, dir);
  mestr_apndc(path, '/');
  mestr_apnd(path, name);
  return me_file_new(mestr_extrd(path), S_IFREG);
}

bool mefil_patrnmatch(const char* pattern, char* str)
{
  uint32_t len1 = strlen(pattern);
  uint32_t len2 = strlen(str);

  uint32_t j = 0;
  
  for (uint32_t i = 0; i < len2; i++)
  {
    if (j >= len1)
      return false;
    if (pattern[j] == '*')
    {
      j++;
      if (j >= len1)
        return true;
      char end = pattern[j];
      while (i < len2 && str[i] != end)
        i++;
    }
    if (str[i] != pattern[j] && pattern[j] != '?')
      return false;
    j++;
  }
  return j >= len1;
}

struct file** mefil_flist(struct file* file, uint32_t* count)
{
  if (!S_ISDIR(file->type))
    return NULL;

  DIR* dir;
  if ((dir = opendir(file->path)) != NULL)
  {
    *count = 0;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
      if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
	continue;
      *count+=1;
    }

    rewinddir(dir);

    struct file** files = calloc(sizeof(struct file*), *count);
    uint32_t index = 0;

    while ((entry = readdir(dir)) != NULL)
    {
      if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
	continue;

      char* filepath = strjoin(strjoinc(file->path, '/'), entry->d_name);
      files[index] = me_file_new(filepath, 0);
      index++;
    }

    closedir(dir);
    return files;
  }
  return NULL;
}
*/

struct file* me_file_new(char* path)
{
  struct file* file = calloc(sizeof(struct file), 1);
  me_file_init(file, path);
  return file;
}

struct file* me_file_join(char* dir, char* path)
{
  struct string* temp = mestr_new(PATH_MAX);
  mestr_apnd(temp, dir);
  
  if (temp->length > 0 && temp->data[temp->length - 1] != '/')
    mestr_apndc(temp, '/');

  mestr_apnd(temp, path);
  return me_file_new(mestr_extrd(temp));
}

void me_file_init(struct file* file, char* path)
{
  path = me_file_normalize(path);
  uint32_t len = strlen(path);

  file->path = path;
  file->prefix_length = (len > 0 && path[0] == '/') ? 1 : 0;
}

char* me_file_absolute_path(struct file* file)
{
  return realpath(file->path, NULL);
}

char* me_file_directory_path(struct file* file)
{
  int index = strlic(file->path, '/');

  if (index == -1)
    return ".";
  else if (index == 0)
    return "/";
  return strsub(file->path, 0, index);
}

char* me_file_canonical_path(struct file* file)
{
  // TODO:
  return file->path;
}

enum me_file_result me_file_mkdir(struct file* file)
{
  mkdir(file->path, 0700);
  return FILE_SUCCESS;
}

enum me_file_result me_file_mkdirs(struct file* file)
{
  return FILE_SUCCESS;
}

enum me_file_result me_file_mkfile(struct file* file)
{
  return FILE_SUCCESS;
}

bool me_file_exists(struct file* file)
{
  return access(file->path, R_OK) == 0;
}

bool me_file_is_file(struct file* file)
{
  struct stat stat_path;
  stat(file->path, &stat_path);
  return S_ISREG(stat_path.st_mode);
}

bool me_file_is_directory(struct file* file)
{
  struct stat stat_path;
  stat(file->path, &stat_path);
  return S_ISDIR(stat_path.st_mode);
}

bool me_file_is_absolute_path(struct file* file)
{
  return file->prefix_length > 0;
}

uint32_t me_file_list(struct file* file, struct array* files)
{
  if (!me_file_is_directory(file))
    return 0;

  uint32_t count = 0;

  DIR* dir;
  if ((dir = opendir(file->path)) != NULL)
  {
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
      if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
	continue;

      struct file* f = me_file_join(file->path, entry->d_name);

      if (me_file_is_directory(f))
	count += me_file_list(f, files);
      else if (me_file_is_file(f))
	mearr_push(files, f);
    }
    closedir(dir);
  }
  return count;
}



char* me_file_chext(char* path, char* ext)
{
  struct string* buff = mestr_new(PATH_MAX);
  int index = strlic(path, '.');

  uint32_t len = strlen(path);
  for (uint32_t i = 0; i < (index >= 0 ? index : len); i++)
    mestr_apndc(buff, path[i]);

  mestr_apndc(buff, '.');
  mestr_apnd(buff, ext);
  return mestr_extrd(buff);
}

enum me_file_result me_file_write(char* file, void* data, uint32_t len)
{
  FILE* f = fopen(file, "wb");
  if (f == NULL)
    return FILE_FAILED_OPEN;

  fwrite(data, 1, len, f);

  fclose(f);
  return FILE_SUCCESS;
}

enum me_file_result me_file_read(char* file, void** data, uint32_t* len)
{
  FILE* f = fopen(file, "rb");
  if (f == NULL)
    return FILE_FAILED_OPEN;

  /* get the file size */
  fseek(f, 0L, SEEK_END);
  *len = ftell(f);

  rewind(f);

  *data = malloc(*len);
  fread(*data, 1, *len, f);

  fclose(f);
  return FILE_SUCCESS;
}

char* me_file_normalize(char* path)
{
  struct string* fixed = mestr_new(PATH_MAX);

  uint32_t len = strlen(path);

  for (uint32_t i = 0; i < len; i++)
  {
    char c = path[i];

    if (i > 0 && c == '/' && path[i - 1] == '/')
      continue;

    if (i > 2 && i < len - 1 && c == '.' && path[i + 1] == '/')
    {
      i++;
      continue;
    }

    mestr_apndc(fixed, c);
  }
  return mestr_extrd(fixed);
}
