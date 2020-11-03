#include "Token.hpp"

#include <cstring>

MI::Token::Token(Type type, void* value, Location location)
{
  this->type = type;
  this->value.v = value;
  this->location = location;
}
