#include "YCMConfig.hpp"

#include <Util.hpp>

makeit::ycm::YCMConfig::YCMConfig()
{
}

void makeit::ycm::YCMConfig::put_flag(Flag* flag)
{
  flags.push_back(flag);
}

int makeit::ycm::YCMConfig::write(std::string &str)
{
  str.append("def Settings(**kwargs):\n");
  str.append("\treturn {\n");
  str.append("\t\t\'flags\': [\n");
  for (uint32_t i = 0; i < flags.size(); i++)
  {
    const Flag* flag = flags.at(i);
    if (flag->get_entries().size() == 0)
      continue;

    str.append("\t\t\t");
    wrtie_flag(str, flag);
    if (i < flags.size() - 1)
      str += ',';
    str += '\n';
  }
  str.append("\t\t]\n\t}\n");
  return 1;
}

void makeit::ycm::YCMConfig::wrtie_flag(std::string &str, const Flag* flag)
{
  const std::vector<std::string> &entries = flag->get_entries();
  for (uint32_t i = 0; i < entries.size(); i++)
  {
    std::string element = entries.at(i);
    makeit::string_replace(element, " ", "\\ ");
    makeit::string_replace(element, "\"", "\\\"");

    str += '\'';
    str.append(flag->get_prefix()).append(element);
    str += '\'';

    if (i < entries.size() - 1)
      str.append(", ");
  }
}
