#ifndef MAKEIT_PROJECT_HPP
  #define MAKEIT_PROJECT_HPP

#include "configs/BuildConfig.hpp"

namespace makeit {

  struct Project {

    std::string name = "untitled_project";
    std::string version = "";
    std::string language = "c";

    BuildConfig config;

  };

}

#endif
