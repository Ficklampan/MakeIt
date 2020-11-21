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
    case LITERIAL: return "literial";
    case CALL: return "call";
    case CONSTANT: return "constant";
    case IF: return "if";
    case THEN: return "then";
    case ELSE: return "else";
    case END: return "end";
    case PUNCTUATOR: return "punctuator";
    case BREAK: return "break";
  }
  return "";
}
