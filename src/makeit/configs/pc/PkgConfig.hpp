#ifndef MAKEIT_PC_PKGCONFIG_HPP
  #define MAKEIT_PC_PKGCONFIG_HPP

#include "Variable.hpp"
#include "Flag.hpp"

#include <vector>

namespace makeit { namespace pc {

  class PkgConfig {

  private:

    std::vector<Variable*> variables;
    std::vector<Flag*> flags;

  public:

    PkgConfig();

    PkgConfig(std::vector<Variable*> &variables, std::vector<Flag*> &flags);

    void put_variable(Variable* var);
    void put_flag(Flag* flag);

    int write(std::string &str);

  private:

    void write_variable(std::string &str, const Variable* var);
    void write_flag(std::string &str, const Flag* flag);

  };

} }

#endif
