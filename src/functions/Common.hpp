#ifndef COMMON_HPP
  #define COMMON_HPP

#include "../script/Variable.hpp"

namespace MI {

  static inline void APPEND_STRINGS(MI::Constant* constant, std::vector<std::string*> &strs)
  {
    if (constant->type == MI::Constant::STRING)
      strs.push_back(constant->value.s);
    else if (constant->type == MI::Constant::LIST)
    {
      for (MI::Constant* c : *constant->value.l)
        APPEND_STRINGS(c, strs);
    }
  }

#ifndef REQUIRE_VARIABLE
  #define REQUIRE_VARIABLE(n, t, c) \
    std::string req_var_name = std::string(#n); \
    MI::Constant* n_var = (MI::Constant*) storage->variables[req_var_name]; \
    if (n_var == nullptr || n_var->type != t) \
    { \
      info = new char[128]; \
      sprintf(info, "required variable '%s' not found", req_var_name.c_str()); \
      return 0; \
    } \
    c n = (c) n_var->value.v;
#endif

}

#endif
