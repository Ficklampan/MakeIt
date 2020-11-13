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
      BOOLEAN,
      LIST,
      STRUCT
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

    static inline const char* typeName(Type type)
    {
      switch (type)
      {
	case VOID: return "void";
	case STRING: return "string";
	case INTEGER: return "integer";
	case BOOLEAN: return "boolean";
	case LIST: return "list";
	case STRUCT: return "struct";
      }
      return "?";
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
