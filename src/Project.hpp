#ifndef MI_PROJECT_HPP
  #define MI_PROJECT_HPP

#include <string>
#include <vector>

namespace MI {

  class Project {

  public:

    std::string name;
    std::string version;
    std::string language;

    std::vector<std::string*> libraries;
    std::vector<std::string*> includes;
    std::vector<std::string*> library_paths;
    std::vector<std::string*> include_paths;
    std::vector<std::string*> definitions;

  };

}

#endif
