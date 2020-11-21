#include "Variable.hpp"

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

int makeit::util::variable_check_struct(Variable* var, const std::map<std::string, Variable::Type> &expected, char* &info)
{
  Variable::v_struct &st = *var->as_struct();

  for (auto &[key, value] : expected)
  {
    Variable* v = st[key];
    if (v == nullptr)
    {
      info = new char[32 + key.size()];
      sprintf(info, "member '%s' not found in struct", key.c_str());
      return 0;
    }
    
    /* [Error] type not match */
    if (value != Variable::VOID && v->type != value)
    {
      info = new char[64];
      sprintf(info, "expected type '%s' but found type '%s'", Variable::type_name(value), Variable::type_name(v->type));
      return 0;
    }
  }

  return 1;
}
