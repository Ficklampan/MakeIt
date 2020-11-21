#ifndef MAKEIT_WILDCARD_HPP
  #define MAKEIT_WILDCARD_HPP

#include <string>

namespace makeit {

  struct Wildcard {

    enum Type : uint8_t {
      DEFINED = 1
    } type;
    std::string value;

    Wildcard(Type type, std::string value)
    {
      this->type = type;
      this->value = value;
    }

  };

}

#endif
