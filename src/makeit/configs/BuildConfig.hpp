#ifndef MAKEIT_BUILD_CONFIG_HPP
  #define MAKEIT_BUILD_CONFIG_HPP

#include "ExternConfig.hpp"

#include <lme/file.hpp>

#include <string>
#include <vector>
#include <map>

namespace makeit {

  struct BuildConfig {

    enum OutType {
      EXECUTABLE, STATIC_LIBRARY, SHARED_LIBRARY
    };

    std::string cc = "gcc";
    me::File build = std::string("build");
    std::string out_name = "";
    OutType out_type = EXECUTABLE;

    std::vector<std::string> cflags;
    std::vector<std::string> lflags;
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
