#ifndef MAKE_FILE_H
  #define MAKE_FILE_H

#include "../Project.hpp"

namespace MI {

  int GNUMake_config(const std::string &filepath, std::vector<std::string> &sources, std::vector<std::string> &headers, Project& project);

}


#endif
