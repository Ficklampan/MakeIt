#ifndef MI_VARIABLE_HPP
  #define MI_VARIABLE_HPP

#include <cstdio>
#include <vector>
#include <string>

namespace MI {

  struct Constant {

    enum Type {
      VOID,
      STRING,
      INTEGER,
      LIST
    } type;
    union {
      void* v;
      std::string* s;
      int* i;
      std::vector<Constant*>* l;
    } value;

    Constant(Type type, void* value)
    {
      this->type = type;
      this->value.v = value;
    }

    ~Constant()
    {
    }

  };

  struct Variable : Constant {

    Variable(Type type, void* value) : Constant(type, value)
    {
    }

    ~Variable()
    {
    }

    Variable* operator+=(Constant &_const);
    Variable* operator-=(Constant &_const);
    Variable* operator*=(Constant &_const);
    Variable* operator/=(Constant &_const);
    Variable* operator%=(Constant &_const);

  };

}

#endif
