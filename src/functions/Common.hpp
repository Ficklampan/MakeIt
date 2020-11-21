#ifndef COMMON_HPP
  #define COMMON_HPP

#include "../script/Storage.hpp"

namespace makeit {

  static inline void APPEND_STRINGS(Variable* variable, std::vector<std::string*> &strs, Storage* storage)
  {
    if (variable->type == Variable::LIST)
    {
      strs.reserve(variable->as_list()->size());
      for (Variable* v : *variable->as_list())
        APPEND_STRINGS(v, strs, storage);
    }else if (variable->type == makeit::Variable::STRING)
      strs.push_back(variable->as_string());
  }

  static inline int LOOP_VARIABLES(Variable* variable, std::function<int(Variable*)> callback)
  {
    if (variable->type == Variable::LIST)
    {
      for (Variable* v : *variable->as_list())
      {
	if (!callback(v))
	  return 0;
      }
    }else
      return callback(variable);
    return 0;
  }

#ifndef REQUIRE_VARIABLE
  #define REQUIRE_VARIABLE(n, t) \
    std::string req_var_name(#n); \
    makeit::Variable* n_var = (makeit::Variable*) storage->variables[req_var_name]; \
    if (n_var == nullptr || n_var->type != t) \
    { \
      info = new char[128]; \
      sprintf(info, "required variable '%s' not found", req_var_name.c_str()); \
      return 0; \
    }
#endif

}

#endif
