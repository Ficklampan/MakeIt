#ifndef MI_YCM_H
  #define MI_YCM_H

#include "Config.hpp"

namespace mi {

  int YCM_config(const std::string &filepath, std::vector<std::string> &sources, std::vector<std::string> &headers, Config* config);

}


#endif
