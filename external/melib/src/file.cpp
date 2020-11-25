#include "../include/lme/file.hpp"
#include "../include/lme/util.hpp"

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>

me::File::File(const std::string &path)
{
  this->path = path;
  normalize(this->path);
  prefixLength = this->path.size() > 0 && this->path.at(0) == FILE_SEPARATOR ? 1 : 0;
}

me::File::File(const std::string &parent, const std::string &child)
{
  path.assign(parent);
  
  if (path.size() > 0 && path.at(path.size() - 1) != FILE_SEPARATOR)
    path += FILE_SEPARATOR;

  path.append(child);

  normalize(path);
  prefixLength = this->path.size() > 0 && this->path.at(0) == FILE_SEPARATOR ? 1 : 0;
}

const std::string me::File::absolutePath() const
{
  std::string real_path(PATH_MAX, '\0');
  realpath(path.c_str(), real_path.data());
  return real_path;
}

const std::string me::File::directoryPath() const
{
  size_t index = path.rfind(FILE_SEPARATOR);

  if (index == -1)
    return ".";
  else if (index == 0)
    return std::string(1, FILE_SEPARATOR);

  return path.substr(0, index);
}

const std::string me::File::canonicalPath() const
{
  std::string real_path(PATH_MAX, '\0');
  realpath(path.c_str(), real_path.data());
  return real_path;
}

int me::File::parentPath(std::string &path) const
{
  size_t index = this->path.rfind(FILE_SEPARATOR);

  if (index == std::string::npos || index < prefixLength)
  {
    if (prefixLength > 0)
    {
      path = this->path.substr(0, prefixLength);
      return 1;
    }
    return 0;
  }

  path = this->path.substr(0, index);
  return 1;
}

int me::File::mkdir() const
{
  return ::mkdir(path.c_str(), 0700) == 0;
}

int me::File::mkdirs() const
{
  if (exists())
    return 0;
  else if (mkdir())
    return 1;
  else
  {
    std::string parent_path;
    if (!parentPath(parent_path))
      return 0;

    File parent(parent_path);

    return (parent.mkdirs() || parent.exists()) && mkdir();
  }
}

int me::File::mkfile() const
{
  return 1;
}

bool me::File::exists() const
{
  return access(path.c_str(), R_OK) == 0;
}

bool me::File::isFile() const
{
  if (!exists())
    return false;
  return S_ISREG(getType());
}

bool me::File::isAbsolutePath() const
{
  return prefixLength > 0;
}

bool me::File::isRelativePath() const
{
  char cwd[PATH_MAX + 1];
  getcwd(cwd, PATH_MAX + 1);
  size_t len = strlen(cwd);

  std::string abs = absolutePath();
  return abs.size() >= len && abs.find(cwd) == 0;
}

bool me::File::isDirectory() const
{
  if (!exists())
    return false;
  return S_ISDIR(getType());
}

const std::string& me::File::getPath() const
{
  return path;
}

int me::File::getType() const
{
  struct stat stat_path;
  stat(path.c_str(), &stat_path);
  return stat_path.st_mode;
}

size_t me::File::listFiles(bool sub_dirs, std::function<int(File&)> &callback) const
{
  if (!isDirectory())
    return 0;

  size_t count = 0;

  DIR* dir;
  if ((dir = opendir(path.c_str())) != nullptr)
  {
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
      if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
	continue;

      File file(path, entry->d_name);

      if (sub_dirs && file.isDirectory())
	count += file.listFiles(sub_dirs, callback);
      else if (file.isFile())
      {
	count++;
	if (!callback(file))
	  break;
      }
    }
    closedir(dir);
  }

  return count;
}


int me::File::read(const File &file, void* &data, uint32_t &size)
{
  FILE* f = fopen(file.getPath().c_str(), "rb");
  if (f == NULL)
    return 0;

  fseek(f, 0L, SEEK_END);
  size = ftell(f);

  rewind(f);

  data = malloc(size);
  fread(data, 1, size, f);

  fclose(f);

  return 1;
}

int me::File::write(const File &file, void* data, uint32_t size)
{
  FILE* f = fopen(file.getPath().c_str(), "wb");
  if (f == NULL)
    return 0;

  fwrite(data, 1, size, f);

  fclose(f);

  return 1;
}

void me::File::normalize(std::string &path)
{
  std::string fixed;

  for (uint32_t i = 0; i < path.size(); i++)
  {
    char c = path.at(i);

    if (i > 0 && c == FILE_SEPARATOR && path.at(i - 1) == FILE_SEPARATOR)
      continue;

    if (i > 2 && i < path.size() - 1 && c == '.' && path[i + 1] == FILE_SEPARATOR)
    {
      i++;
      continue;
    }

    fixed += c;
  }

  path.assign(fixed);
}

/*
 * '/home/me/.stuff/testcpp' gonna explode [fixed?]
 */
std::string me::File::extension(const std::string &filename, const std::string &ext)
{
  std::string new_str;
  new_str.reserve(PATH_MAX);
  std::size_t index = filename.rfind('.');
  std::size_t index2 = filename.rfind(FILE_SEPARATOR);

  if (index != std::string::npos && index2 != std::string::npos && index < index2)
    index = std::string::npos;

  for (uint32_t i = 0; i < (index == std::string::npos ? filename.size() : index); i++)
    new_str += filename.at(i);

  new_str += '.';
  new_str.append(ext);
  return new_str;
}
