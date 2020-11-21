#ifndef MI_PARSER_HPP
  #define MI_PARSER_HPP

#include "Token.hpp"
#include "Storage.hpp"

#include <lme/iterator.hpp>

#include <vector>
#include <map>

namespace makeit {

  class Parser {

  public:

    static int parse(me::BasicIterator<Token*> &tokens, Storage* storage);

  private:

    static int parse_args(me::BasicIterator<Token*> &tokens, Storage* storage, std::vector<Variable*> &args);

  };

}

#endif
