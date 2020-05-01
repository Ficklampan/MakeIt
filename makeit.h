#ifndef MAKEIT_H
  #define MAKEIT_H

#include <string>
#include <map>
#include <vector>

namespace makeit {

  struct execute {
    std::string sources, libs;
    std::string version;
  };

  struct project {
    std::string name;
    std::map<std::string, std::string> vars;
    std::string includeDirs;
    std::string libraryDirs;
    execute* exec;

    project(std::string name)
    {
      this->name = name;
    }

  };

};

#endif
