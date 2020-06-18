#include "file_utils.h"
#include <stdio.h>
#include <filesystem>

unsigned char* file_utils::read_file(const std::string &filepath)
{
  FILE* file = fopen(filepath.c_str(), "rb");
  fseek(file, 0, SEEK_END);
  unsigned int file_size = ftell(file);
  unsigned char* buffer = new unsigned char[file_size];
  rewind(file);
  fread((char*) buffer, file_size, 1, file);
  fclose(file);
  return buffer;
}

void file_utils::write_file(const std::string &filepath, unsigned char* data, unsigned int len)
{
  FILE* file = fopen(filepath.c_str(), "wb");
  fwrite(data, 1, len, file);
  fclose(file);
}

void file_utils::get_file_name(const std::string &filepath, std::string &name, std::string &ext)
{
  uint32_t lastSplit = 0;
  for (uint32_t i = 0; i < filepath.size(); i++)
  {
    char c = filepath.at(i);
    if (c == '/')
      lastSplit = i;
  }
  bool readExt = false;
  for (uint32_t i = lastSplit + 1; i < filepath.size(); i++)
  {
    char c = filepath.at(i);
    if (c == '.')
    {
      readExt = true;
      continue;
    }
    if (readExt)
      ext+=c;
    else
      name+=c;
  }
}

std::vector<std::string> file_utils::find_files(const std::string &directory, const std::string &fileName, const std::string &fileExt)
{
  std::vector<std::string> files;
  for (const auto &entry : std::filesystem::directory_iterator(directory))
  {
    std::string entryName, entryExt;
    get_file_name(entry.path().string(), entryName, entryExt);
    if (fileName=="**" && entry.is_directory())
    {
      std::vector<std::string> sub = find_files(entry.path().string() + "/", fileName, fileExt);
      files.insert(files.end(), sub.begin(), sub.end());
    }
    if (entry.is_directory())
      continue;
    bool nameMatch = false, extMatch = false;

    if (fileName == "*" || fileName == "**")
      nameMatch = true;
    else if (fileName == entryName)
      nameMatch = true;

    if (fileExt == "*")
      extMatch = true;
    else if (fileExt.compare(entryExt))
      extMatch = true;

    if (nameMatch && extMatch)
      files.push_back(entry.path().string());
  }
  return files;
}
