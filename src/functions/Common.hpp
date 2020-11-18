#ifndef COMMON_HPP
  #define COMMON_HPP

#include "../script/Variable.hpp"

namespace MI {

  static inline void APPEND_STRINGS(MI::VariableRef* variable, std::vector<std::string*> &strs)
  {
    if (variable->type == MI::VariableRef::STRING)
      strs.push_back(&((VarString*) variable)->value);
    else if (variable->type == MI::VariableRef::LIST)
    {
      for (MI::VariableRef* c : ((VarList*) variable)->value)
        APPEND_STRINGS(c, strs);
    }
  }

#ifndef REQUIRE_VARIABLE
  #define REQUIRE_VARIABLE(n, t) \
    std::string req_var_name = std::string(#n); \
    MI::VariableRef* n_var = (MI::VariableRef*) storage->variables[req_var_name]; \
    if (n_var == nullptr || n_var->type != t) \
    { \
      info = new char[128]; \
      sprintf(info, "required variable '%s' not found", req_var_name.c_str()); \
      return 0; \
    }
#endif

}

#endif
