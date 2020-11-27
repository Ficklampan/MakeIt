#ifndef MI_CONFIG_HPP
  #define MI_CONFIG_HPP

#define MIDEBUG(l, ...) if (config.debug >= l) { /*printf("'%s'[%u]: ", __FILE__, __LINE__);*/ printf(__VA_ARGS__); }

#include "Text.hpp"

#include <string>
#include <vector>
#include <variant>

namespace makeit {

  struct Config {
    std::string file;
    bool print_all;
    bool block_all;
    uint8_t debug;
    std::vector<std::string> configs;
  };

  typedef std::variant<const char*, char, int, uint32_t, float, double> ExceptionArg;
  template<typename L>
  class Exception : public std::exception {

  private:

    const L location;
    const Error err;
    const std::vector<ExceptionArg> args;

  public:

    Exception(const L &location, const Error err, const std::vector<ExceptionArg> &args);

    const L get_location() const;
    const Error get_err() const;
    const std::vector<ExceptionArg>& get_args() const;
    const char* get_message() const;

  };

}

#include "Config_Impl.hpp"

#endif
