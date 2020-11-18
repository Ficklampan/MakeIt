#include "Token.hpp"

#include <cstring>

MI::Token::Token(Type type, void* value, Location location)
{
  this->type = type;
  this->value.v = value;
  this->location = location;
}

const char* MI::Token::typeName(Type type)
{
  switch (type)
  {
    case LITERIAL: return "LITERIAL";
    case CONSTANT: return "CONSTANT";
    case PUNCTUATOR: return "PUNCTUATOR";
    case BREAK: return "BREAK";
  }
  return "UNKNOWN";
}
