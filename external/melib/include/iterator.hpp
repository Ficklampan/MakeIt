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
        printf("struct (Iterator) index out of bounds. %u/%u\n", index, size); abort(); \
      })
  #endif
  #ifndef ME_ITERATOR_CHECK_ERROR
    #define ME_ITERATOR_CHECK_ERROR() ({ \
        if (index >= size) ME_ITERATOR_PRINT_ERROR_IOB(); \
      })
  #endif
#endif

namespace me {

  template<typename T>
  struct Iterator {

    T* entries;
    uint32_t size;
    uint32_t index;

    Iterator(T* entries, uint32_t size)
    {
      this->entries = entries;
      this->size = size;
      index = 0;
    }

    Iterator(std::vector<T> &entries)
    {
      Iterator(entries.data(), entries.size());
    }

    inline T& next()
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

}

#endif
