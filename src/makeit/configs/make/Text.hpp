#ifndef MAKEIT_GNUMAKE_TEXT_HPP
  #define MAKEIT_GNUMAKE_TEXT_HPP

#include "Element.hpp"

#include <string>
#include <vector>

namespace makeit { namespace gnu_make {

  class Text : public Element {

  private:

    const std::vector<std::string> lines;
    const bool comment;

  public:

    Text(const std::vector<std::string> &lines, const bool comment, const int pos = -2)
      : Element(Element::TEXT, pos), lines(lines), comment(comment)
    {
    }

    const std::vector<std::string>& get_lines() const
    {
      return lines;
    }
    
    const bool is_comment() const
    {
      return comment;
    }

  };

} }

#endif
