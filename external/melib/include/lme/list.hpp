#ifndef MELIB_LIST_HPP
  #define MELIB_LIST_HPP

#include <cstddef>

namespace me {

  template<typename T>
  class LinkedList {

  private:

    T value;
    LinkedList* next;

  public:

    LinkedList()
    {
      next = nullptr;
    }

    size_t size() const
    {
      size_t size = 0;
      if (next != nullptr)
	size += next->size();
      return size;
    }

    T& at(size_t index) const
    {
      if (index == 0)
	return value;

      if (next != nullptr)
	return next->at(index - 1);
      return nullptr;
    }

  };

}

#endif
