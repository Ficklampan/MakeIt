#ifndef MELIB_FIL_H
  #define MELIB_FIL_H

#include "metype.h"
#include "mearr.h"

#include <sys/stat.h>

#define IS_ABSOLUTE_PATH(f) \
  (f[0] == '/')

struct file {
  char* path;
  uint8_t prefix_length;

  struct {
    int type;
    char* abs_path;
    char* directory;
  } cache;
};

enum me_file_result {
  FILE_DIR_EXISTS = -8,
  FILE_FILE_EXISTS = -7,
  FILE_FAILED_OPEN = -6,
  FILE_FAILED_READ = -5,
  FILE_FAILED_WRITE = -4,
  FILE_NO_PERMISSION = -3,
  FILE_NOT_FOUND = -2,
  FILE_NO_SPACE = -1,
  FILE_SUCCESS = 1
};

/*
 * Create new file instance and initialize
 */
struct file* me_file_new(char* path);

struct file* me_file_join(char* dir, char* path);

/*
 * Initialize file instance
 */
void me_file_init(struct file* file, char* path);

/*
 * Get the absolute path of the file/directory
 * - type 'regular file' / 'directory'
 */
char* me_file_absolute_path(struct file* file);

/*
 * Get the directory of a file
 * - type 'regular file' / 'directory'
 */
char* me_file_directory_path(struct file* file);

/*
 * Get the canonical path of a file
 */
char* me_file_canonical_path(struct file* file);

/*
 * Create directory if it does not exists
 * - type 'directory'
 */
enum me_file_result me_file_mkdir(struct file* file);

/*
 * Create directory and missing directories if it does not exists
 * - type 'directory' / 'regular file'
 */
enum me_file_result me_file_mkdirs(struct file* file);

/*
 * Create file if it does not exists
 * - type 'regular file'
 */
enum me_file_result me_file_mkfile(struct file* file);

/*
 * Check if file or directory exists
 * - type 'regular file' / 'directory'
 */
bool me_file_exists(struct file* file);

/*
 * Check if file is file
 */
bool me_file_is_file(struct file* file);

/*
 * Check if file is directory
 */
bool me_file_is_directory(struct file* file);

/*
 * Check if file path is absolute path
 */
bool me_file_is_absolute_path(struct file* file);

/*
 * Get list of files in directory
 * - type 'directory'
 */
uint32_t me_file_list(struct file* file, struct array* files);



/*
 * Change file name extension
 */
char* me_file_chext(char* path, char* ext);

/*
 * Wrtie data to file
 * - type 'regular file'
 */
enum me_file_result me_file_write(char* file, void* data, uint32_t len);

/*
 * Read data from file
 * - type 'regular file'
 */
enum me_file_result me_file_read(char* file, void** data, uint32_t* len);

char* me_file_normalize(char* path);

#endif
