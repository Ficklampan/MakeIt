#ifndef MELIB_ITERATOR_HPP
  #define MELIB_ITERATOR_HPP

#ifndef MELIB_NO_ERR_PRINT
  #include <stdio.h>
  #include <stdlib.h>
#endif

#include <cstdint>
#include <vector>

#ifndef MELIB_NO_ERR_PRINT
  #ifndef ME_ITERATOR_PRINT_ERROR_IOB
    #define ME_ITERATOR_PRINT_ERROR_IOB() ({ \
        printf("struct (Iterator) index out of bounds. %u/%u\n", this->index, this->size); abort(); \
      })
  #endif
  #ifndef ME_ITERATOR_CHECK_ERROR
    #define ME_ITERATOR_CHECK_ERROR() ({ \
        if (this->index >= this->size) ME_ITERATOR_PRINT_ERROR_IOB(); \
      })
  #endif
#endif

namespace me {

  template<typename T>
  struct BasicIterator {

    T* entries;
    uint32_t size;
    uint32_t index;

    BasicIterator(T* entries, uint32_t size)
    {
      this->entries = entries;
      this->size = size;
      index = 0;
    }

    BasicIterator(std::vector<T> &entries)
    {
      BasicIterator(entries.data(), entries.size());
    }

    inline virtual T& next()
    {
      #ifndef MELIB_NO_ERR_PRINT
	ME_ITERATOR_CHECK_ERROR();
      #endif
      return entries[index++];
    }

    inline T& peek()
    {
      #ifndef MELIB_NO_ERR_PRINT
	ME_ITERATOR_CHECK_ERROR();
      #endif
      return entries[index];
    }

    inline bool hasNext()
    {
      return index < size;
    }

  };

  template<typename T>
  struct Iterator : BasicIterator<T> {

    void (*func) (void*, T&, uint32_t&);
    void* ptr;

    Iterator(T* entries, uint32_t size, void (*func) (void*, T&, uint32_t&), void* ptr) : BasicIterator<T>(entries, size)
    {
      this->func = func;
      this->ptr = ptr;
    }

    Iterator(std::vector<T> &entries, void (*func) (void*, T&, uint32_t&), void* ptr)
    {
      Iterator(entries.data(), entries.size(), func, ptr);
    }

    inline T& next() override
    {
#ifndef MELIB_NO_ERR_PRINT
  ME_ITERATOR_CHECK_ERROR();
#endif
      if (func != nullptr)
	func(ptr, this->entries[this->index], this->index);
      return this->entries[this->index++];
    }

  };

}

#endif
