#ifndef MAKEIT_BUILD_CONFIG_HPP
  #define MAKEIT_BUILD_CONFIG_HPP

#include <lme/file.hpp>

#include <string>
#include <vector>
#include <map>

namespace makeit {

  struct BuildConfig {

    std::string cc = "gcc";
    me::File build = std::string("build");

    std::vector<std::string> flags;
    std::vector<std::string> libraries;
    std::vector<std::string> includes;
    std::vector<std::string> library_paths;
    std::vector<std::string> include_paths;
    std::map<std::string, std::string> definitions;
    std::vector<std::string> sources;

  };

}

#endif
