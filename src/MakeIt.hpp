#ifndef MAKEIT_HPP
  #define MAKEIT_HPP

#include "script/Storage.hpp"

#include <lme/file.hpp>

namespace MI {

  struct Config {
    std::string file;
    bool print_time;
  } config = {
    .file = "MIBuild",
    .print_time = false
  };

  int parseArgs(int argc, char** argv);
  int initScript();
  int readScript(me::File &file, Storage &storage);

}

#endif
