#ifndef MI_VARIABLE_HPP
  #define MI_VARIABLE_HPP

#include <cstdio>
#include <vector>
#include <string>

namespace MI {

  struct Constant {

    enum Type : uint8_t {
      VOID = 1,
      STRING = 2,
      INTEGER = 3,
      BOOLEAN = 4,
      LIST = 5,
      STRUCT = 6
    } type;
    union {
      void* v;
      std::string* s;
      int* i;
      bool* b;
      std::vector<Constant*>* l;
    } value;

    Constant(Type type, void* value)
    {
      this->type = type;
      this->value.v = value;
    }

    ~Constant()
    {
      if (type == STRING)
	delete value.s;
      else if (type == INTEGER)
	delete value.i;
      else if (type == BOOLEAN)
	delete value.b;
      else if (type == LIST)
      {
	for (Constant* c : *value.l)
	  delete c;
	delete value.l;
      }else
	free(value.v);
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

    Variable* operator+=(Constant &_const);
    Variable* operator-=(Constant &_const);
    Variable* operator*=(Constant &_const);
    Variable* operator/=(Constant &_const);
    Variable* operator%=(Constant &_const);

  };

}

#endif
