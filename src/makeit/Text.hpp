#ifndef MAKEIT_TEXT_HPP
  #define MAKEIT_TEXT_HPP

#include "Common.hpp"

#include <cstring>

namespace makeit {

  enum Error {
    EUNDEFINED_VARIABLE,
    EUNDEFINED_REFERENCE,
    EUNDEFINED_FUNCTION,
    ETOO_FEW_ARGS,
    ETOO_MANY_ARGS,
    EEXPECTED_VALUE,
    EEXPECTED_CONSTANT,
    EEXPECTED_TOKEN,
    EEXPECTED_TYPE_AT_ARG,
    EEXPECTED_TYPE_AT_MEMBER,
    EEXPECTED_VALUE_TYPE,
    EEXPECTED_TOKEN_TYPE,
    EEXPECTED_EXPRESSION,
    EEXPECTED_IDENTIFIER,
    EUNKNOWN_SYMBOL,
    EMAKEFILE,
    ELAST
  };

  enum Warning {
    WWILD_TOKEN
  };

  const char* get_error(const Error err);
  const char* get_error_id(const Error err);
  const char* get_warning(const Warning warn);
  const char* get_warning_id(const Warning warn);

}

#endif
