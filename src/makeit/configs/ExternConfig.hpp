#ifndef MAKEIT_EXTERN_CONFIG_HPP
  #define MAKEIT_EXTERN_CONFIG_HPP

#include <string>

namespace makeit {

  struct ExternConfig {

    enum Type {
      MAKEIT, CMAKE, PREMAKE, GNUMAKE, SHELL
    };

    const Type type;
    const std::string name;
    const std::string target;
    const std::string args;

    ExternConfig(const Type type, const std::string &name, const std::string &target, const std::string &args)
      : type(type), name(name), target(target), args(args)
    {
    }

  };

}

#endif
