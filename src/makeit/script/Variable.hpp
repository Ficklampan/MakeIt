#ifndef MAKEIT_VARIABLE_HPP
  #define MAKEIT_VARIABLE_HPP

#include "TokenLocation.hpp"

#include <cstdio>
#include <vector>
#include <string>
#include <map>

namespace makeit {

  struct Variable {

    enum Type : uint8_t {
      VOID = 1 /* used as 'any' type */, STRING = 2, INTEGER = 3, LIST = 4, STRUCT = 5, POINTER = 6, REFERENCE = 7, LITERIAL = 8
    };

    /* variable value types */
    typedef std::string v_string;
    typedef int v_integer;
    typedef std::vector<Variable*> v_list;
    typedef std::map<std::string, Variable*> v_struct;
    typedef void* v_pointer;
    typedef std::string v_reference;
    typedef std::string v_literial;
    /* -------------------- */

    TokenLocation* location;
    Type type;
    void* value;

    Variable(TokenLocation* location, Type type, void* value = nullptr)
    {
      this->location = location;
      this->type = type;
      this->value = value;
    }

#define VARIABLE_CHECK_TYPE(t) { if (t != type) printf("!!! trying to cast type '%s' to type '%s'\n", type_name(type), type_name(t)); }
    v_string* as_string() const { VARIABLE_CHECK_TYPE(STRING); return (v_string*) value; }
    v_integer* as_integer() const { VARIABLE_CHECK_TYPE(INTEGER); return (v_integer*) value; }
    v_list* as_list() const { VARIABLE_CHECK_TYPE(LIST); return (v_list*) value; }
    v_struct* as_struct() const { VARIABLE_CHECK_TYPE(STRUCT); return (v_struct*) value; }
    /* useless */ v_pointer* as_pointer() const { VARIABLE_CHECK_TYPE(POINTER); return (v_pointer*) value; }
    v_reference* as_reference() const { VARIABLE_CHECK_TYPE(REFERENCE); return (v_reference*) value; }
    v_literial* as_literial() const { VARIABLE_CHECK_TYPE(LITERIAL); return (v_literial*) value; }
#undef VARIABLE_CHECK_TYPE

    /* equals (v)ariable */ bool operator==(Variable* v);
    /* append/addition (v)ariable */ Variable* operator+=(Variable* v);
    /* remove/subtract (v)ariable */ Variable* operator-=(Variable* v);
    /* multiply with (v)ariable*/ Variable* operator*=(Variable* v);
    /* divide with (v)ariable */ Variable* operator/=(Variable* v);
    /* 'modulo' for integers */ Variable* operator%=(Variable* v);

    static const char* type_name(Type type);

  };

  class Argument {
    
  public:

    enum Flag {
      ENDLESS = 1,
      OPTIONAL = 1 << 1
    };

  protected:

    const Variable::Type type;
    const uint8_t flags;

  public:

    Argument(const Variable::Type type, const uint8_t flags = 0)
      : type(type), flags(flags)
    {
    }

    const Variable::Type get_type() const
    {
      return type;
    }

    const bool is_endless() const
    {
      return flags & ENDLESS;
    }

    const bool is_optional() const
    {
      return flags & OPTIONAL;
    }

    virtual void match(Variable* var) const;

  };

  class StructArg : public Argument {

  public:

    struct Arg {

      const std::string name;
      const bool required;
      const Argument* arg;

      Arg(const std::string &name, const bool required, const Argument* arg)
	: name(name), required(required), arg(arg)
      {
      }

    };

  private:

    const std::vector<Arg> args;

  public:

    StructArg(const std::vector<Arg>& args, const uint8_t flags = 0)
      : Argument(Variable::STRUCT, flags), args(args)
    {
    }

    void match(Variable* var) const override;

  };

  class ListArg : public Argument {

  private:

    const Argument* arg_type;

  public:

    ListArg(const Argument* arg_type, const uint8_t flags = 0)
      : Argument(Variable::LIST, flags), arg_type(arg_type)
    {
    }

    void match(Variable* var) const override;

  };

}

#endif
