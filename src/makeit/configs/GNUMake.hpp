#ifndef MAKEIT_FILE_HPP
  #define MAKEIT_FILE_HPP

#include "BuildConfig.hpp"

#include <lme/file.hpp>

#include <vector>
#include <map>

namespace makeit {

  class GNUMake {

  public:

    struct Options {
      bool comments = true;
      std::vector<std::string*> depends;
    };

  private:

    const std::string name;
    const BuildConfig* config;
    const Options options;

    int write_header(std::string &str);
    int write_depends(std::string &str);
    int write_footer(std::string &str);

  public:

    GNUMake(const std::string &name, const BuildConfig* config, const Options &options);

    int generate(std::string &str);

  };

}

#endif
