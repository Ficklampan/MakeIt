#ifndef MAKEIT_HPP
  #define MAKEIT_HPP

#include "Project.hpp"

#include "script/Storage.hpp"

#include <mefile.hpp>

namespace MI {

  struct Config {
    std::string file;
    bool print_time;
  } config = {
    .file = "MIBuild",
    .print_time = false
  };

  int parseArgs(int argc, char** argv);
  void initProject(Project &project);
  void initStorage(Storage &storage, Project* project);
  int initScript();
  int readScript(me::File &file, Storage &storage);

}

#endif
