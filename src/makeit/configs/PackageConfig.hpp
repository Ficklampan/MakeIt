#ifndef MAKEIT_PACKAGE_CONFIG_HPP
  #define MAKEIT_PACKAGE_CONFIG_HPP

#include <vector>
#include <string>

namespace makeit {

  struct LPackageConfig {

    const bool static_;
    const std::vector<std::string> flags;
    const std::string name;

    LPackageConfig(const bool static_, const std::vector<std::string> &flags, const std::string &name)
      : static_(static_), flags(flags), name(name)
    {
    }

  };

  struct CPackageConfig {

    const std::vector<std::string> flags;
    const std::string name;

    CPackageConfig(const std::vector<std::string> &flags, const std::string &name)
      : flags(flags), name(name)
    {
    }

  };

}

#endif
