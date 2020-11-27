#ifndef MAKEIT_GNUMAKE_MAKEFILE_HPP
  #define MAKEIT_GNUMAKE_MAKEFILE_HPP

#include "Variable.hpp"
#include "Rule.hpp"
#include "Text.hpp"

#include <variant>
#include <string>

namespace makeit { namespace gnu_make {

  class Makefile {

  private:

    std::vector<Element*> elements;

  public:

    Makefile();

    void put_element(Element* element);

    int write(std::string &str);
    void write_variable(std::string &str, const Variable* var);
    void write_rule(std::string &str, const Rule* rule);
    void write_text(std::string &str, const Text* text);

  };

} }

#endif
