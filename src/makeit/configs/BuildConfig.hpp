#ifndef MAKEIT_BUILD_CONFIG_HPP
  #define MAKEIT_BUILD_CONFIG_HPP

#include "ExternConfig.hpp"
#include "PackageConfig.hpp"

#include <lme/file.hpp>

#include <string>
#include <vector>
#include <map>

namespace makeit {

  struct BuildConfig {

    enum Kind {
      EXECUTABLE, STATIC_LIBRARY, SHARED_LIBRARY
    };

    enum Lang {
      C, CPP
    };

    enum Optimization {
      NONE, O1, O2, O3, Os, Ofast, Og, Oz
    };

    enum CPPVersion {
      CPP_98, CPP_11, CPP_14, CPP_17, CPP_20
    };

    enum CC {
      GNU, LLVM
    };


    me::File build = std::string("build");
    Lang lang = C;
    CC cc = GNU;
    Kind kind = EXECUTABLE;
    Optimization optimization = NONE;
    CPPVersion cpp_version = CPP_14;

    std::vector<std::string> flags;
    std::vector<std::string> libraries;
    std::vector<std::string> includes;
    std::vector<std::string> library_paths;
    std::vector<std::string> include_paths;
    std::vector<std::string> definitions;
    std::vector<std::string> sources;
    std::vector<ExternConfig> extern_configs;
    std::vector<LPackageConfig> lpackage_configs;
    std::vector<CPackageConfig> cpackage_configs;

  };

}

#endif
