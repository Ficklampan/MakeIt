#include "../include/lme/util.hpp"

size_t me::string::split(const std::string &str, char c, std::vector<std::string_view> &strs)
{
  size_t count = 1;

  for (size_t i = 0; i < str.size(); i++)
  {
    if (str.at(i) == c)
      count++;
  }

  strs.reserve(count);

  size_t index = 0;

  for (size_t i = 0; i < str.size(); i++)
  {
    if (str.at(i) == c)
    {
      strs.emplace_back(str.c_str() + index, i - index);
      index = i + 1;
      continue;
    }
  }
  strs.emplace_back(str.c_str() + index, str.size() - index);

  return count;
}
