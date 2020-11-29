#include "PkgConfig.hpp"

#include "Util.hpp"

makeit::pc::PkgConfig::PkgConfig()
{
}

makeit::pc::PkgConfig::PkgConfig(std::vector<Variable*> &variables, std::vector<Flag*> &flags)
{
  this->variables = variables;
  this->flags = flags;
}

void makeit::pc::PkgConfig::put_variable(Variable* var)
{
  variables.push_back(var);
}

void makeit::pc::PkgConfig::put_flag(Flag* flag)
{
  flags.push_back(flag);
}

int makeit::pc::PkgConfig::write(std::string &str)
{
  for (const Variable* var : variables)
    write_variable(str, var);
  str += '\n';
  for (const Flag* flag : flags)
    write_flag(str, flag);
  return 1;
}

void makeit::pc::PkgConfig::write_variable(std::string &str, const Variable* var)
{
  str.append(var->get_name()).append("=").append(var->get_value()) += '\n';
}

void makeit::pc::PkgConfig::write_flag(std::string &str, const Flag* flag)
{
  str.append(flag->get_name()).append(": ");
  const std::vector<std::string> &value = flag->get_value();
  for (uint32_t i = 0; i < value.size(); i++)
  {
    std::string element = value.at(i);
    makeit::string_replace(element, " ", "\\ ");
    makeit::string_replace(element, "\"", "\\\"");

    str.append(element);
    if (i < value.size() - 1)
      str += ' ';
  }
  str += '\n';
}
