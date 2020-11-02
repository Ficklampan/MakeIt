#include "Variable.hpp"

MI::Variable* MI::Variable::operator+=(Constant &_const)
{
  if (type == LIST)
  {
    if (_const.type == LIST)
    {
      for (auto v : *_const.value.l)
        value.l->push_back(v);
    }else
    {
      value.l->push_back(&_const);
    }

  }else if (type == STRING)
  {
    if (_const.type == STRING)
    {
      value.s->append(*_const.value.s);
    }

  }else if (type == INTEGER)
  {
    if (_const.type == INTEGER)
    {
      *value.i += *_const.value.i;
    }
  }

  return this;
}

MI::Variable* MI::Variable::operator-=(Constant &_const)
{
  if (type == INTEGER)
  {
    if (_const.type == INTEGER)
    {
      *value.i -= *_const.value.i;
    }
  }

  return this;
}

MI::Variable* MI::Variable::operator*=(Constant &_const)
{
  if (type == INTEGER)
  {
    if (_const.type == INTEGER)
    {
      *value.i *= *_const.value.i;
    }
  }

  return this;
}

MI::Variable* MI::Variable::operator/=(Constant &_const)
{
  if (type == INTEGER)
  {
    if (_const.type == INTEGER)
    {
      *value.i /= *_const.value.i;
    }
  }

  return this;
}

MI::Variable* MI::Variable::operator%=(Constant &_const)
{
  if (type == INTEGER)
  {
    if (_const.type == INTEGER)
    {
      *value.i %= *_const.value.i;
    }
  }

  return this;
}
