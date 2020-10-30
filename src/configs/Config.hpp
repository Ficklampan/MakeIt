#ifndef MI_CONFIG_HPP
  #define MI_CONFIG_HPP

#include <string>
#include <vector>

#include <me/mefile.hpp>

namespace mi {

  struct Config {
    std::string name;
    std::string version;
    std::string language;
    std::string compiler;

    me::File* build_dir;

    std::vector<std::string> flag;
    std::vector<std::string> library;
    std::vector<std::string> include;
    std::vector<std::string> library_dir;
    std::vector<std::string> include_dir;
    std::vector<std::string> definitions;
  };

}

#endif
