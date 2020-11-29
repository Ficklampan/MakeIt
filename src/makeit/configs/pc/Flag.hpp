#ifndef MAKEIT_PC_FLAG_HPP
  #define MAKEIT_PC_FLAG_HPP

#include <string>
#include <vector>

namespace makeit { namespace pc {

  class Flag {

  private:

    const std::string name;
    const std::vector<std::string> value;

  public:

    Flag(const std::string &name, const std::vector<std::string> &value)
      : name(name), value(value)
    {
    }

    Flag(const std::string &name, const std::string &value)
      : name(name), value({ value })
    {
    }

    const std::string& get_name() const
    {
      return name;
    }

    const std::vector<std::string>& get_value() const
    {
      return value;
    }

  };

} }

#endif
