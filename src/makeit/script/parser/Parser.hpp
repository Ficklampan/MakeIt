#ifndef MAKEIT_PARSER_HPP
  #define MAKEIT_PARSER_HPP

#include "../Token.hpp"
#include "../Storage.hpp"

#include <lme/iterator.hpp>

#include <vector>
#include <map>

namespace makeit {

  class Parser {

  public:

    enum Flag {
      NO_BREAK = 1,
      NO_ERROR = 1 << 1,
      NO_WARNING = 1 << 2
    };

    static int get_args(me::Iterator<Token*> &tokens, Storage* storage, std::vector<Variable*> &args, uint8_t flags);
    static int get_variable(Token* token, me::Iterator<Token*> &tokens, Storage* storage, Variable* &variable, uint8_t flags);

    static int parse_tokens(me::Iterator<Token*> &tokens, Storage* storage, uint8_t flags);
    static int parse_token(Token* token, me::Iterator<Token*> &tokens, Storage* storage, uint8_t flags);

  private:

    static int parse_literial(Token* token, me::Iterator<Token*> &tokens, Storage* storage, uint8_t flags);
    static int parse_statement(Token* token, me::Iterator<Token*> &tokens, Storage* storage, uint8_t flags);
    static int parse_function(Token* token, me::Iterator<Token*> &tokens, Storage* storage, uint8_t flags);

  };

}

#endif
