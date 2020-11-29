#ifndef MAKEIT_YCM_FLAG_HPP
  #define MAKEIT_YCM_FLAG_HPP

#include <string>
#include <vector>

namespace makeit { namespace ycm {

  class Flag {

  private:

    const std::string prefix;
    const std::vector<std::string> entries;

  public:

    Flag(const std::string &prefix, const std::vector<std::string> &entries)
      : prefix(prefix), entries(entries)
    {
    }

    Flag(const std::string &prefix, const std::string &entry)
      : prefix(prefix), entries({ entry })
    {
    }

    const std::string& get_prefix() const
    {
      return prefix;
    }

    const std::vector<std::string>& get_entries() const
    {
      return entries;
    }

  };

} }

#endif
