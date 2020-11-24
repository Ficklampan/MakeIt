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
  }
  return "";
}

