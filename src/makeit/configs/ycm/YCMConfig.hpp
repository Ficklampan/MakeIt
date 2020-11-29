#ifndef MAKEIT_YCM_YCMCONFIG_HPP
  #define MAKEIT_YCM_YCMCONFIG_HPP

#include "Flag.hpp"

namespace makeit { namespace ycm {

  class YCMConfig {

  private:

    std::vector<Flag*> flags;

  public:

    YCMConfig();

    void put_flag(Flag* flag);

    int write(std::string &str);

  private:

    void wrtie_flag(std::string &str, const Flag* flag);

  };

} }

#endif
