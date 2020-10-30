#ifndef MAKE_FILE_H
  #define MAKE_FILE_H

#include "Config.hpp"

namespace mi {

  int GNUMake_config(const std::string &filepath, std::vector<std::string> &sources, std::vector<std::string> &headers, Config* config);

}


#endif
