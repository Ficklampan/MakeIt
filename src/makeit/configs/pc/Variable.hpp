#ifndef MAKEIT_PC_VARIABLE_HPP
  #define MAKEIT_PC_VARIABLE_HPP

#include <string>

namespace makeit { namespace pc {

  class Variable {

  private:

    const std::string name;
    const std::string value;

  public:

    Variable(const std::string &name, const std::string &value)
      : name(name), value(value)
    {
    }

    const std::string& get_name() const
    {
      return name;
    }

    const std::string& get_value() const
    {
      return value;
    }

  };

} }

#endif
