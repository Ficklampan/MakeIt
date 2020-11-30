#include "Variable.hpp"

#include "../Config.hpp"

bool makeit::Variable::operator==(makeit::Variable* v)
{
  if (type == INTEGER)
  {
    int i1 = *this->as_integer();

    if (v->type == INTEGER)
    {
      int i2 = *v->as_integer();
      return i1 == i2;
    }
  }else if (type == STRING)
  {
    std::string* str1 = this->as_string();

    if (v->type == STRING)
    {
      std::string* str2 = v->as_string();
      return str1->compare(*str2) == 0;
    }
  }
  return false;
}

makeit::Variable* makeit::Variable::operator+=(makeit::Variable* v)
{
  if (type == LIST)
  {
    v_list* list1 = this->as_list();

    /* push all elements from 'v' list1 */
    if (v->type == LIST)
    {
      v_list* list2 = v->as_list();
      for (Variable* e : *list2)
	list1->push_back(e);

    /* push 'v' to list1 */
    }else
    {
      list1->push_back(v);
    }

  }else if (type == STRING)
  {
    v_string* string = this->as_string();

    /* appending content from 'v' to 'this' string */
    if (v->type == STRING)
    {
      string->append(*v->as_string());
    }
  }
  return this;
}

makeit::Variable* makeit::Variable::operator-=(makeit::Variable* v)
{
  return this;
}

makeit::Variable* makeit::Variable::operator*=(makeit::Variable* v)
{
  return this;
}

makeit::Variable* makeit::Variable::operator/=(makeit::Variable* v)
{
  return this;
}

makeit::Variable* makeit::Variable::operator%=(makeit::Variable* v)
{
  return this;
}

const char* makeit::Variable::type_name(Type type)
{
  switch (type)
  {
    case VOID: return "void";
    case STRING: return "string";
    case INTEGER: return "integer";
    case LIST: return "list";
    case STRUCT: return "struct";
    case POINTER: return "pointer";
    case REFERENCE: return "reference";
    case LITERIAL: return "literial";
  }
  return "";
}

void makeit::Argument::match(Variable* var) const
{
  if (type == Variable::VOID)
    return;
  if (var->type != type)
    throw Exception<TokenLocation*>(var->location, EEXPECTED_VALUE_TYPE, { Variable::type_name(type), Variable::type_name(var->type) });
}

void makeit::StructArg::match(Variable* var) const
{
  Argument::match(var);

  Variable::v_struct &st = *var->as_struct();

  for (const Arg arg : args)
  {
    bool found = st.find(arg.name) != st.end();
    if (arg.required && !found)
      throw Exception<TokenLocation*>(var->location, EEXPECTED_MEMBER, { arg.name.c_str() });
    else if (!found)
      continue;

    arg.arg->match(st[arg.name]);
  }
}

void makeit::ListArg::match(Variable* var) const
{
  Argument::match(var);

  Variable::v_list &list = *var->as_list();

  for (Variable* var2 : list)
  {
    arg_type->match(var2);
  }
}

#undef VARIABLE_MATCH
