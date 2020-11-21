#include "Util.hpp"

void makeit::string_replace(std::string &str, const std::string &target, const std::string &with)
{
  size_t index = 0;
  while (index != std::string::npos && index < str.size())
  {
    index = str.find(target, index);
    if (index == std::string::npos)
      break;

    str.replace(index, target.size(), with);
  }
}
