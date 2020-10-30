#ifndef MI_PARSER_HPP
  #define MI_PARSER_HPP

#include "../Script.hpp"

#include "../lexer/Token.hpp"

#include "../vm/Instruction.hpp"

#include <me/iterator.hpp>

#include <vector>

namespace mi {

  class Parser {

  private:

    me::Iterator<Token*>* tokens;
    std::vector<Instruction*> instructions;

  public:

    Parser(me::Iterator<Token*>* tokens);

    std::vector<Instruction*>& getInstructions();

    int parseTokens();
    int parseTokens(std::vector<Instruction*> &instructions);
    int parseToken(std::vector<Instruction*> &instructions);


  };

}

#endif
