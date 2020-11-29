#ifndef MAKEIT_PROJECT_HPP
  #define MAKEIT_PROJECT_HPP

#include "configs/BuildConfig.hpp"

namespace makeit {

  struct Project {

    std::string name;
    std::string description = "";
    std::string version = "";
    std::string language = "c";

    BuildConfig config;

  };

}

#endif
