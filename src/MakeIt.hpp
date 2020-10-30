#ifndef MAKEIT_HPP
  #define MAKEIT_HPP

#include "configs/Config.hpp"
#include "script/Script.hpp"

namespace mi {

  class Project {

  public:

    Config config;
    Storage storage;

  };

  int readScript(me::File* file, const std::string &source, Project* project);
  int readScript(me::File* file, Project* project);

}

#endif
