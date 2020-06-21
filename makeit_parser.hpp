#ifndef MAKEIT_PARSER_HPP
  #define MAKEIT_PARSER_HPP

#include "makeit.hpp"
#include "triggers/trigger.hpp"

namespace makeit {

  struct Parser {

    std::vector<Trigger*> triggers;

    int lastTriggerStatus = -1;

    int init();
    int parse(makeit::project* project, std::vector<std::string> &lines, std::string &directory, unsigned int offset, unsigned int length);
    int parse(makeit::project* project, std::vector<std::string> &lines, std::string &directory);
    int parse(makeit::project* project, std::string &directory);
    int make_makefile(makeit::project* project, std::string &directory);

  };

}

#endif
