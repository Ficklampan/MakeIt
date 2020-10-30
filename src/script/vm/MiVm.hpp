#ifndef MI_VM_HPP
  #define MI_VM_HPP

#include "Instruction.hpp"

#include "../Script.hpp"

namespace mi {

  class VM {

  private:

    enum Flag {
      FLAG_ASSIGN = 1,
      FLAG_APPEND = 1 << 1,
      FLAG_ARG = 1 << 2,
    };

    Storage* storage;
    std::vector<Instruction*>* instructions;
    struct State {

      uint64_t status = 0;
      uint32_t count = 0;
      Variable* v = nullptr;
      Function* f = nullptr;
      std::vector<Variable*> args;

      bool getFlag(Flag flag);
      void setFlag(Flag flag, bool val);

    } state;

  public:

    VM(Storage* storage, std::vector<Instruction*>* instructions);

    int clock();
    int execute(Instruction* instr);

  };

}

#endif
