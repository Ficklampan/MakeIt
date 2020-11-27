#ifndef MAKEIT_GNUMAKE_VARIABLE_HPP
  #define MAKEIT_GNUMAKE_VARIABLE_HPP

#include "Element.hpp"

#include <string>
#include <vector>

namespace makeit { namespace gnu_make {

  class Constant : public Element {

  private:

    const std::vector<std::string> value;

  public:

    Constant(const std::vector<std::string> &value, const int pos = -2)
      : Element(Element::VARIABLE, pos), value(value)
    {
    }

    Constant(const std::string &value, const int pos = -2)
      : Element(Element::VARIABLE, pos), value({ value })
    {
    }

    const std::vector<std::string>& get_value() const
    {
      return value;
    }

    virtual const bool is_global() const
    {
      return false;
    }

  };

  class Variable : public Constant {

  private:

    const std::string name;

  public:

    Variable(const std::string &name, const std::vector<std::string> &value, const int pos = -2)
      : Constant(value, pos), name(name)
    {
    }

    Variable(const std::string &name, const std::string &value, const int pos = -2)
      : Constant(value, pos), name(name)
    {
    }

    const std::string& get_name() const
    {
      return name;
    }

    const bool is_global() const override
    {
      return true;
    }

  };

} }

#endif
