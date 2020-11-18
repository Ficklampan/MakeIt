#ifndef MAKE_FILE_H
  #define MAKE_FILE_H

#include "../Project.hpp"

#include <lme/file.hpp>
#include <lme/string.hpp>

namespace MI {

  int GNUMake_config(const std::string &filepath, const std::string &compiler, me::File &build_dir, std::vector<std::string*> &flags, std::vector<std::string*> &sources, std::vector<std::string*> &headers, Project* project);

}

#endif
