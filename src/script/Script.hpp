#ifndef MI_SCRIPT_HPP
  #define MI_SCRIPT_HPP

#include <vector>
#include <map>
#include <string>

namespace mi {

  struct Variable {

    enum Flag : uint8_t {
      CONSTANT = 1,
      ARRAY = 1 << 1,
      UNUSED = 1 << 2
    };

  private:

    union Value {
      void* v;
      std::vector<void*>* a;
    } value;
    uint8_t flags;

  public:

    Variable(void* value, uint8_t flags = 0);

    Value& getValue();
    void setValue(void* val);

    bool getFlag(Flag flag);
    void setFlag(Flag flag, bool val);

  };

  struct Function {

    int (*exec) (std::vector<Variable*>&);
    std::vector<uint8_t> args;

    Function(int (*exec) (std::vector<Variable*>&), std::vector<uint8_t> args);

  };

  class Storage {

  public:

    std::map<std::string, Variable*> variables;
    std::map<std::string, Function*> functions;

  };

}

#endif
