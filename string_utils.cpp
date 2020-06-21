#include "string_utils.hpp"

bool string_utils::starts_with(const std::string &string, const std::string &with)
{
  if (with.size() > string.size())
    return false;
  for (unsigned int i = 0; i < with.length(); i++)
  {
    if (string.at(i) != with.at(i))
      return false;
  }
  return true;
}

/* splitting string */
std::vector<std::string> string_utils::split(const std::string &str, char delimeter)
{
  std::vector<std::string> strings;
  unsigned int start = 0, end = 0;
  for (unsigned int i = 0; i < str.size(); i++)
  {
    if (str.at(i)==delimeter)
    {
      strings.emplace_back(&str.at(start), end);
      start = i + 1;
      end = 0;
      continue;
    }
    end++;
  }
  if (start < str.size())
    strings.emplace_back(&str.at(start));
  return strings;
}

/* stolen from stackoverflow */
/* replaces port of string with new string */
void string_utils::replace(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

/* set the extension of file */
std::string string_utils::file_extension(const std::string &string, const std::string &new_extension)
{
  unsigned int end = string.size() - 1;
  for (unsigned int i = end; i >= 0; i--)
  {
    if (string.at(i) == '.')
    {
      end = i;
      break;
    }
  }
  return string.substr(0, end) + "." + new_extension;
}

/* get directory of file */
std::string string_utils::directory(const std::string &filepath)
{
  unsigned int end = filepath.size() - 1;
  for (unsigned int i = end; i >= 0; i--)
  {
    if (filepath.at(i) == '/')
    {
      end = i;
      break;
    }
  }
  return filepath.substr(0, end);
}

std::string string_utils::fix_path(const std::string &filepath, bool folder)
{
  std::string fixed;
  fixed.append(filepath);
  replace(fixed, "\\", "/");
  if (folder && filepath.at(filepath.size() - 1) != '/')
    fixed+='/';
  return fixed;
}

/* get index of char */
unsigned int string_utils::index(std::string &string, char c)
{
  for (unsigned int i = 0; i < string.size(); i++)
  {
    if (string.at(i) != c)
      return i;
  }
  return 0;
}
