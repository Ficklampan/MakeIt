#ifndef PROJECT_TRIGGER_HPP
  #define PROJECT_TRIGGER_HPP

#include "trigger.hpp"

namespace makekit {

  struct VariableTrigger : Trigger {
    inline int triggered(makeit::project* project, const std::string &prefix, const std::vector<std::string> &elements, unsigned int index)
    {
      if (prefix == "project")
      {
        if (elements.size() < 1)
        {
          printf(ERR_NO_NAME_SPEC, index + 1);
          return 2;
        }
        project = new makeit::project(elements.at(0));
        return 1;
      }else if (prefix == "directory")
      {
        if (elements.size() < 1)
        {
          printf(ERR_NO_DIR_SPEC, index + 1);
          return 2;
        }
        
        return 1;
      }
      return 0;
    }
  };

}

#endif
