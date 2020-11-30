#ifndef MAKEIT_EXTERN_CONFIG_HPP
  #define MAKEIT_EXTERN_CONFIG_HPP

#include <string>
#include <vector>

namespace makeit {

  struct ExternConfig {

    const std::string name;
    const std::string target;
    const std::string prerequisites;
    const std::vector<std::string> commands;
    const bool phony;

    ExternConfig(const std::string &name, const std::string &target, const std::string &prerequisites, const std::vector<std::string> &commands, const bool phony)
      : name(name), target(target), prerequisites(prerequisites), commands(commands), phony(phony)
    {
    }

  };

}

#endif
