#ifndef MAKEIT_PARSER
  #define MAKEIT_PARSER

#include "makeit.h"

namespace makeit {

  int parse(makeit::project* project, std::vector<std::string> lines, std::string &directory, unsigned int level, unsigned int offset, unsigned int length);
  int parse(makeit::project* project, std::vector<std::string> lines, std::string &directory);
  int parse(makeit::project* project, std::string &directory);
  int makefile(makeit::project* project, std::string &directory);

};

#endif
