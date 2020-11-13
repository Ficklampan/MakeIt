#ifndef MI_FUNCTION_HPP
  #define MI_FUNCTION_HPP

#include "Variable.hpp"

namespace MI {

  struct Arg {

    std::vector<uint8_t> type;
    bool endless;

    Arg(std::vector<uint8_t> type, bool endless)
    {
      this->type = type;
      this->endless = endless;
    }

  };

  struct ArgBuilder {

    enum Flag : uint8_t {
      FLAG_ANY = 1,
      ENDLESS = 1 << 1,
    };

    uint8_t flags;
    std::vector<Arg*> args;

    ArgBuilder() { }

    ArgBuilder& type(Constant::Type type)
    {
      args.push_back(new Arg({(uint8_t)type}, false));
      return *this;
    }

    ArgBuilder& type(std::vector<uint8_t> types)
    {
      args.push_back(new Arg(types, false));
      return *this;
    }

    ArgBuilder& any()
    {
      args.push_back(new Arg({Constant::VOID}, false));
      return *this;
    }

    std::vector<Arg*> endless()
    {
      args.at(args.size() - 1)->endless = true;
      return args;
    }

    std::vector<Arg*> build()
    {
      return args;
    }


  };

  struct Function {

    std::string desc;
    std::vector<Arg*> args;

    virtual int execute(void* ptr, std::vector<Constant*> &args, char* &info) = 0;

  };

}

#endif
