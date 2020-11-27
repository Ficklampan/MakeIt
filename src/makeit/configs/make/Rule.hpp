#ifndef MAKEIT_GNUMAKE_RECIPE_HPP
  #define MAKEIT_GNUMAKE_RECIPE_HPP

#include "Element.hpp"

#include "Command.hpp"

namespace makeit { namespace gnu_make {

  class Rule : public Element {

  public:

    enum Flag {
      PHONY = 1
    };

  private:

    const std::string target;
    const std::string prerequisites;
    const std::vector<Command> commands;
    const uint8_t flags;

  public:

    Rule(const std::string &target, const std::string &prerequisites, const std::vector<Command> &commands, const uint8_t flags, const int pos = -2)
      : Element(Element::RULE, pos), target(target), prerequisites(prerequisites), commands(commands), flags(flags)
    {
    }

    const std::string& get_target() const
    {
      return target;
    }

    const std::string& get_prerequisites() const
    {
      return prerequisites;
    }

    const std::vector<Command>& get_commands() const
    {
      return commands;
    }

    const bool is_phony() const
    {
      return flags & PHONY;
    }

  };

} }

#endif
