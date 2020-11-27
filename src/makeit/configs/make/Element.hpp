#ifndef MAKEIT_GNUMAKE_ELEMENT_HPP
  #define MAKEIT_GNUMAKE_ELEMENT_HPP

namespace makeit { namespace gnu_make {

  class Element {

  public:

    enum Type : unsigned char {
      VARIABLE, RULE, TEXT
    };

  private:

    const Type type;
    int pos;

  public:

    Element(const Type type, int pos = -2)
      : type(type)
    {
      this->pos = pos;
    }

    const Type get_type() const
    {
      return type;
    }

    const int get_pos() const
    {
      return pos;
    }

    void set_pos(int pos)
    {
      this->pos = pos;
    }

  };

} }

#endif
