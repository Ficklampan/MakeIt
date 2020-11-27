#ifndef MAKEIT_GNUMAKE_COMMAND_HPP
  #define MAKEIT_GNUMAKE_COMMAND_HPP

#include "Element.hpp"

#include <string>
#include <vector>

namespace makeit { namespace gnu_make {

  struct Command {

    const std::string args;

    Command(const std::string &args)
      : args(args)
    {
    }

  };

} }

#endif
