#ifndef MAKEIT_FUNCUTILS_HPP
  #define MAKEIT_FUNCUTILS_HPP

#include "../script/Token.hpp"
#include "../script/Storage.hpp"

#include "../Config.hpp"

namespace makeit { namespace function {

  static inline void GET_STRINGS(Variable* variable, std::vector<std::string> &strs, const std::string &prefix = "", const std::string &suffix = "")
  {
    if (variable->type == Variable::LIST)
    {
      strs.reserve(variable->as_list()->size());
      for (Variable* v : *variable->as_list())
        GET_STRINGS(v, strs, prefix, suffix);
    }else if (variable->type == makeit::Variable::STRING)
      strs.push_back(prefix + *variable->as_string() + suffix);
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


  static inline int ERROR_CHECK_STRUCT(Variable::v_struct &provided, std::map<std::string, uint16_t> &expected)
  {
    for (auto &[key, value] : expected)
    {
      Variable* var = provided.find(key) == provided.end() ? nullptr : provided[key];
      bool opt = (value & Variable::OPTIONAL) > 0;
      if (var == nullptr)
      {
        if (opt)
  	continue;
        return -2;
      }
  
      Variable::Type type = (Variable::Type) ((value >> 2) & 0xF);
  
      if (type != Variable::VOID && type != var->type)
        throw Exception(0, EEXPECTED_TYPE_AT_MEMBER, { Variable::type_name(type), key.c_str(), Variable::type_name(var->type) });
    }
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
