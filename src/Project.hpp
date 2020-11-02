#ifndef MI_PROJECT_HPP
  #define MI_PROJECT_HPP

#include <mefile.hpp>

#include <string>
#include <vector>

namespace MI {

  class Project {

  public:

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
