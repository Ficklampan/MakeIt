#include "Variable.hpp"

int MI::VariableRef::assign(MI::VariableRef* value)
{
#ifndef VARIABLE_ASSIGN
  #define VARIABLE_ASSIGN(t) { ((t*) this)->value = ((t*) value)->value; }
#endif

  if (type == STRING && value->type == STRING) VARIABLE_ASSIGN(VarList)
  else if (type == INTEGER && value->type == INTEGER) VARIABLE_ASSIGN(VarInteger)
  else if (type == BOOLEAN && value->type == BOOLEAN) VARIABLE_ASSIGN(VarBoolean)
  else if (type == LIST && value->type == LIST) VARIABLE_ASSIGN(VarList)
  else if (type == STRUCT && value->type == STRUCT) VARIABLE_ASSIGN(VarStruct)
  else
    return 0;

  return 1;
}

MI::VariableRef* MI::VariableRef::operator+=(VariableRef* _const)
{
  if (type == LIST)
  {
    VarList* list1 = (VarList*) this;
    if (_const->type == LIST)
    {
      VarList* list2 = (VarList*) _const;
      for (auto v : list2->value)
        list1->value.push_back(v);
    }else
    {
      list1->value.push_back(_const);
    }

  }else if (type == STRING)
  {
    VarString* str1 = (VarString*) this;
    if (_const->type == STRING)
    {
      VarString* str2 = (VarString*) _const;
      str1->value.append(str2->value);
    }

  }else if (type == INTEGER)
  {
    VarInteger* int1 = (VarInteger*) this;
    if (_const->type == INTEGER)
    {
      VarInteger* int2 = (VarInteger*) _const;
      int1->value += int2->value;
    }
  }

  return this;
}

MI::VariableRef* MI::VariableRef::operator-=(VariableRef* _const)
{
  if (type == INTEGER)
  {
    VarInteger* int1 = (VarInteger*) this;
    if (_const->type == INTEGER)
    {
      VarInteger* int2 = (VarInteger*) _const;
      int1->value -= int2->value;
    }
  }

  return this;
}

MI::VariableRef* MI::VariableRef::operator*=(VariableRef* _const)
{
  if (type == INTEGER)
  {
    VarInteger* int1 = (VarInteger*) this;
    if (_const->type == INTEGER)
    {
      VarInteger* int2 = (VarInteger*) _const;
      int1->value *= int2->value;
    }
  }

  return this;
}

MI::VariableRef* MI::VariableRef::operator/=(VariableRef* _const)
{
  if (type == INTEGER)
  {
    VarInteger* int1 = (VarInteger*) this;
    if (_const->type == INTEGER)
    {
      VarInteger* int2 = (VarInteger*) _const;
      int1->value /= int2->value;
    }
  }

  return this;
}

MI::VariableRef* MI::VariableRef::operator%=(VariableRef* _const)
{
  if (type == INTEGER)
  {
    VarInteger* int1 = (VarInteger*) this;
    if (_const->type == INTEGER)
    {
      VarInteger* int2 = (VarInteger*) _const;
      int1->value %= int2->value;
    }
  }

  return this;
}

MI::VariableRef* MI::VariableRef::copy(const VariableRef* ref)
{
  if (ref->type == STRING) return new VarString(((VarString*) ref)->value);
  else if (ref->type == INTEGER) return new VarInteger(((VarInteger*) ref)->value);
  else if (ref->type == BOOLEAN) return new VarBoolean(((VarBoolean*) ref)->value);
  else if (ref->type == LIST) return new VarList(((VarList*) ref)->value);
  else if (ref->type == STRUCT) return new VarStruct(((VarStruct*) ref)->value);
  return nullptr;
}
