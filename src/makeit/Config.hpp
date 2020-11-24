#ifndef MI_CONFIG_HPP
  #define MI_CONFIG_HPP

#define MIDEBUG(l, ...) if (config.debug >= l) { /*printf("'%s'[%u]: ", __FILE__, __LINE__);*/ printf(__VA_ARGS__); }

#include "Text.hpp"

#include <string>
#include <vector>
#include <any>

namespace makeit {

  struct Config {
    std::string file;
    bool print_all;
    bool block_all;
    uint8_t debug;
    std::vector<std::string> configs;
  };

  template<typename L>
  class Exception : public std::exception {

  private:

    const L location;
    const int err;
    const std::vector<std::any> args;

  public:

    Exception(const L &location, const int err, const std::vector<std::any> &args) :
      location(location), err(err), args(args)
    {
    }

    const L get_location() const
    {
      return location;
    }

    const int get_err() const
    {
      return err;
    }

    const std::vector<std::any>& get_args() const
    {
      return args;
    }

    const char* what() const throw()
    {
      char* str = new char[4096];
      const char* err_msg = get_error((Error) err);

      uint32_t len = strlen(err_msg);
      uint32_t index = 0, arg_index = 0;
      for (uint32_t i = 0; i < len; i++)
      {
	if (err_msg[i] == '%')
	{
	  i++;

#define APPEND_ARG(s, t, l) { \
  t arg_data = std::any_cast<t>(args.at(arg_index++)); \
  char arg_data_str[l]; \
  sprintf(arg_data_str, s, arg_data); \
  uint32_t arg_data_str_len = strlen(arg_data_str); \
  for (uint32_t j = 0; j < arg_data_str_len; j++) \
    str[index++] = arg_data_str[j]; \
  continue; \
}

	  if (err_msg[i] == 's') APPEND_ARG("%s", const char*, 4096)
	  else if (err_msg[i] == 'c') APPEND_ARG("%c", char, 2)
	  else if (err_msg[i] == 'u') APPEND_ARG("%u", uint32_t, 25)
	  else if (err_msg[i] == 'i') APPEND_ARG("%i", int, 26)
	  else if (err_msg[i] == 'f') APPEND_ARG("%f", double, 129)
	}

	str[index++] = err_msg[i];
      }
      str[index] = '\0';
      return str;
    }

  };

}

#endif
