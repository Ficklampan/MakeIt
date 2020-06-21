#ifndef VARIABLE_TRIGGER_HPP
  #define VARIABLE_TRIGGER_HPP

#include "trigger.hpp"

namespace makekit {

  struct VariableTrigger : Trigger {
    inline int triggered(makeit::project* project, const std::string &prefix, const std::vector<std::string> &elements, unsigned int index)
    {
      if (prefix == "var" || prefix == "variable")
      {
        if (elements.size() < 1)
        {
          printf(ERR_NO_VAR_SPEC, index + 1)
          return 2;
        }
        for (const std::string &element : elements)
          project->vars[element] = std::string();
        return 1;
      }else if (prefix == "set" || prefix == "append")
      {
        if (elements.size() < 1)
        {
          printf(ERR_NO_VAR_SPEC, index + 1)
          return 2;
        }else if (elements.size() < 2)
        {
          printf(ERR_NO_APPEND, index + 1);
          return 2;
        }
        std::string &append = elements.at(0);
        if (prefix == "set")
          project->vars[append].clear();
        for (unsigned int i = 1; i < elements.size(); i++)
          project->vars[append].append(elements.at(i) + " ");
        return 1;
      }
      return 0;
    }
  };

}

#endif
