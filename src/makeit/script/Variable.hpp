#ifndef MAKEIT_VARIABLE_HPP
  #define MAKEIT_VARIABLE_HPP

#include "TokenLocation.hpp"

#include <cstdio>
#include <vector>
#include <string>
#include <map>

namespace makeit {

  struct Variable {

    enum Arg {
      ENDLESS = 1,
      OPTIONAL = 1 << 2
    };

    /* variable value types */
    typedef std::string v_string;
    typedef int v_integer;
    typedef std::vector<Variable*> v_list;
    typedef std::map<std::string, Variable*> v_struct;
    typedef void* v_pointer;
    typedef std::string v_reference;
    /* -------------------- */

    TokenLocation* location;
    enum Type : uint8_t {
      VOID = 1 /* used as 'any' type */, STRING = 2, INTEGER = 3, LIST = 4, STRUCT = 5, POINTER = 6, REFERENCE = 7
    } type;
    void* value;

    Variable(TokenLocation* location, Type type, void* value = nullptr)
    {
      this->location = location;
      this->type = type;
      this->value = value;
    }

    v_string* as_string() const { return (v_string*) value; }
    v_integer* as_integer() const { return (v_integer*) value; }
    v_list* as_list() const { return (v_list*) value; }
    v_struct* as_struct() const { return (v_struct*) value; }
    /* useless */ v_pointer* as_pointer() const { return (v_pointer*) value; }
    v_reference* as_reference() const { return (v_reference*) value; }

    /* equals (v)ariable */ bool operator==(Variable* v);
    /* append/addition (v)ariable */ Variable* operator+=(Variable* v);
    /* remove/subtract (v)ariable */ Variable* operator-=(Variable* v);
    /* multiply with (v)ariable*/ Variable* operator*=(Variable* v);
    /* divide with (v)ariable */ Variable* operator/=(Variable* v);
    /* 'modulo' for integers */ Variable* operator%=(Variable* v);

    static const char* type_name(Type type);

  };

}

#endif
