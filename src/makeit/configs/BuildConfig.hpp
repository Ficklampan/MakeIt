#ifndef MAKEIT_BUILD_CONFIG_HPP
  #define MAKEIT_BUILD_CONFIG_HPP

#include "ExternConfig.hpp"

#include <lme/file.hpp>

#include <string>
#include <vector>
#include <map>

namespace makeit {

  struct BuildConfig {

    enum OutputType {
      EXECUTABLE, STATIC_LIBRARY, SHARED_LIBRARY
    };

    std::string cc = "gcc";
    me::File build = std::string("build");
    OutputType output_type = EXECUTABLE;

    std::vector<std::string> flags;
    std::vector<std::string> libraries;
    std::vector<std::string> includes;
    std::vector<std::string> library_paths;
    std::vector<std::string> include_paths;
    std::vector<std::string> definitions;
    std::vector<std::string> sources;
    std::vector<ExternConfig> extern_configs;

  };

}

#endif
