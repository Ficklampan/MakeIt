#ifndef MI_OPCODE_HPP
  #define MI_OPCODE_HPP

#include "../Script.hpp"

#include "../lexer/Token.hpp"

namespace mi {

  enum OpCode {
    OPCODE_VARIABLE,
    OPCODE_ARG,

    OPCODE_PREPARE_CALL,
    OPCODE_CALL,

    OPCODE_CONSTANT,

    OPCODE_ASSIGN,
    OPCODE_APPEND,

    OPCODE_RESET
  };

  struct Instruction {

    OpCode code;
    void* value;

    inline Instruction(OpCode code, void* value)
    {
      this->code = code;
      this->value = value;
    }

  };

}

#endif
