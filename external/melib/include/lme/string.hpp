#ifndef MELIB_STRING_HPP
  #define MELIB_STRING_HPP

#include <cstring>

namespace me {

  class String {

  private:

    char* str;
    size_t length;
    String* next = nullptr;

  public:

    String(const char* str, size_t length, String* next = nullptr)
    {
      this->str = (char*) str;
      this->length = length;
      this->next = next;
    }
    
    String(char* str, String* next = nullptr)
    {
      String(str, strlen(str), next);
    }

    String()
    {
      String("", 0, nullptr);
    }

    size_t size() const
    {
      size_t size = length;
      if (next != nullptr)
	size += next->size();
      return size;
    }

    char& at(size_t index) const
    {
      if (index >= length && next != nullptr)
	return next->at(index - length);
      return str[index];
    }

    void append(String* str)
    {
      if (next == nullptr)
	next = str;
      else
	next->append(str);
    }

    char* toString() const
    {
      size_t length = size();
      char* s = new char[length + 1];
      s[length] = '\0';

      for (size_t i = 0; i < length; i++)
	s[i] = at(i);

      return s;
    }

  };

}

#endif
