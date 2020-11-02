#ifndef MI_PARSER_HPP
  #define MI_PARSER_HPP

#include "Token.hpp"
#include "Storage.hpp"

#include <iterator.hpp>

#include <vector>
#include <map>

namespace MI {

  class Parser {

  public:

    int parse(me::Iterator<Token*> &tokens, Storage &storage);
    int parse_args(me::Iterator<Token*> &tokens, Storage &storage, std::vector<Constant*> &args);

  };

}

#endif
