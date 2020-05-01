#ifndef MAKEIT_PARSER
  #define MAKEIT_PARSER

#include "makeit.h"

namespace makeit {

  void parse(makeit::project* project, std::vector<std::string> lines, std::string &directory, unsigned int level, unsigned int offset, unsigned int length);
  void parse(makeit::project* project, std::vector<std::string> lines, std::string &directory);
  void parse(makeit::project* project, std::string &directory);
  void makefile(makeit::project* project, std::string &directory);

};

#endif
