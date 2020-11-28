#ifndef MELIB_ITERATOR_HPP
  #define MELIB_ITERATOR_HPP

#ifndef MELIB_NO_ERR_PRINT
  #include <stdio.h>
  #include <stdlib.h>
#endif

#include <cstdint>
#include <vector>
#include <functional>

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
    size_t size;
    size_t index;

    BasicIterator(T* entries, size_t size)
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

    inline T& last()
    {
      return entries[index - 1];
    }

    inline bool hasNext()
    {
      return index < size;
    }

  };

  template<typename T>
  struct Iterator : BasicIterator<T> {

    std::function<void(T&, size_t&)> callback;

    Iterator(T* entries, uint32_t size, std::function<void(T&, size_t&)> &callback) : BasicIterator<T>(entries, size)
    {
      this->callback = callback;
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
      callback(this->entries[this->index], this->index);
      return this->entries[this->index++];
    }

  };

}

#endif
