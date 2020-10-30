#include "Script.hpp"

mi::Variable::Variable(void* value, uint8_t flags)
{
  this->value.v = value;
  this->flags = flags;
}

mi::Variable::Value& mi::Variable::getValue()
{
  return value;
}

void mi::Variable::setValue(void* val)
{
  if (getFlag(UNUSED))
    setFlag(UNUSED, false);
  value.v = val;
}

bool mi::Variable::getFlag(Flag flag)
{
  return (flags & flag) > 0;
}

void mi::Variable::setFlag(Flag flag, bool val)
{
  if (val)
    flags |= flag;
  else
    flags &= (~flag);
}

mi::Function::Function(int (*exec) (std::vector<Variable*>&), std::vector<uint8_t> args)
{
  this->exec = exec;
  this->args = args;
}
