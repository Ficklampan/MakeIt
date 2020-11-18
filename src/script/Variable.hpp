#ifndef MI_VARIABLE_HPP
  #define MI_VARIABLE_HPP

#include <cstdio>
#include <vector>
#include <string>

#define VARIABLE_STRING(v) (((MI::VarString*) v))
#define VARIABLE_INTEGER(v) (((MI::VarInteger*) v))
#define VARIABLE_BOOLEAN(v) (((MI::VarBoolean*) v))
#define VARIABLE_LIST(v) (((MI::VarList*) v))
#define VARIABLE_STRUCT(v) (((MI::VarStruct*) v))

namespace MI {

  struct VariableRef {

    enum Type : uint8_t {
      VOID = 1,
      STRING = 2,
      INTEGER = 3,
      BOOLEAN = 4,
      LIST = 5,
      STRUCT = 6
    } type;

    VariableRef(Type type)
    {
      this->type = type;
    }

    int assign(VariableRef* value);

    VariableRef* operator+=(VariableRef* _const);
    VariableRef* operator-=(VariableRef* _const);
    VariableRef* operator*=(VariableRef* _const);
    VariableRef* operator/=(VariableRef* _const);
    VariableRef* operator%=(VariableRef* _const);

    static VariableRef* copy(const VariableRef* ref);

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

  template<typename T>
  struct Variable : VariableRef {

    T value;

    Variable(Type type, T value) : VariableRef(type)
    {
      this->type = type;
      this->value = value;
    }

    ~Variable()
    {
    }

  };

  struct VarString : Variable<std::string> 
  { VarString(const std::string &str) : Variable<std::string>(VariableRef::STRING, str) { } };

  struct VarInteger : Variable<int> 
  { VarInteger(const int &i) : Variable<int>(VariableRef::INTEGER, i) { } };

  struct VarBoolean : Variable<bool> 
  { VarBoolean(const bool &b) : Variable<bool>(VariableRef::BOOLEAN, b) { } };

  struct VarList : Variable<std::vector<VariableRef*>>
  { VarList(const std::vector<VariableRef*> &list) : Variable<std::vector<VariableRef*>>(VariableRef::LIST, list) { } };

  struct VarStruct : Variable<void*> 
  { VarStruct(void* st) : Variable<void*>(VariableRef::STRUCT, st) { } };

}

#endif
