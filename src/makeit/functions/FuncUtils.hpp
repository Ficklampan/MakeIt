#ifndef MAKEIT_FUNCUTILS_HPP
  #define MAKEIT_FUNCUTILS_HPP

#include "../script/Token.hpp"
#include "../script/Storage.hpp"

#include "../Config.hpp"

namespace makeit { namespace function {

  static inline void GET_STRINGS(Variable* variable, std::vector<std::string> &strs)
  {
    if (variable->type == Variable::LIST)
    {
      strs.reserve(variable->as_list()->size());
      for (Variable* v : *variable->as_list())
        GET_STRINGS(v, strs);
    }else if (variable->type == makeit::Variable::STRING)
      strs.push_back(*variable->as_string());
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

  static inline int REQUIRE_VARIABLE(const std::string &name, const Variable::Type type, Storage* storage)
  {
    Variable* var = storage->variables[name];
    if (var == nullptr || var->type != type)
      return 0;
    return 1;
  }

} }

#endif
