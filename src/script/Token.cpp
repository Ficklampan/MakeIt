#include "Token.hpp"

#include <cstring>

makeit::Token::Token(Type type, void* value, TokenLocation location)
{
  this->type = type;
  this->value.v = value;
  this->location = location;
}

const char* makeit::Token::type_name(Type type)
{
  switch (type)
  {
    case LITERIAL: return "LITERIAL";
    case CALL: return "CALL";
    case CONSTANT: return "CONSTANT";
    case PUNCTUATOR: return "PUNCTUATOR";
    case BREAK: return "BREAK";
  }
  return "UNKNOWN";
}
