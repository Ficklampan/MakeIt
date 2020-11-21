#ifndef MAKEIT_TEXT_HPP
  #define MAKEIT_TEXT_HPP

#include "Common.hpp"

#include <cstring>

namespace makeit {

  static const char* Eundefined_variable = "undefined variable '%s' [-Eundefined_variable]";
  static const char* Eundefined_reference = "undefined reference '%s' [-Eundefined_reference]";
  static const char* Eundefined_function = "undefined function '%s' [-Eundefined_function]";
  static const char* Ewild_identifier = "found identifier in the wild '%s' [-Ewild_identifier]";
  static const char* Etoo_few_args = "too few arguments [-Etoo_few_args]";
  static const char* Etoo_many_args = "too many arguments [-Etoo_many_args]";
  static const char* Eexpected_value = "expected value [-Eexpected_value]";
  static const char* Eexpected_constant = "expected constant [-Eexpected_constant]";
  static const char* Eexpected_type_at_arg = "expected type '%s' at arg[%u] but found type '%s' [-Eexpected_type_at_arg]";
  static const char* Eexpected_type = "expected type '%s' but found type '%s' [-Eexpected_type]";
  static const char* Eexpected_token = "expected token '%s' [-Eexpected_token]";
  static const char* Eexpected_expression = "expected expression [-Eexpected_expression]";
  static const char* Eexpected_identifier = "expected identifier [-Eexpected_identifier]";
  static const char* Eunknown_symbol = "unknown symbol '%c' [-Eunknown_symbol]";

  static const char* Wwild_token = "found token in the wild [-Wwild_token]";

  static const char* error_info(const char* err)
  {
    if (strcmp(err, "undefined_variable") == 0) return _("tried to access a variable that was not defined");
    else if (strcmp(err, "undefined_reference") == 0) return _("tried to access a variable from name that was not defined");
    else if (strcmp(err, "undefined_function") == 0) return _("tried to call a function that did not exists");
    else if (strcmp(err, "wild_identifier") == 0) return _("found a identifier/literial with no context");
    else if (strcmp(err, "too_few_args") == 0) return _("too few arguments was specified when calling a function");
    else if (strcmp(err, "too_many_args") == 0) return _("too many arguments was specified when calling a function");
    else if (strcmp(err, "expected_value") == 0) return _("expected a value/variable but found nothing or something else");
    else if (strcmp(err, "expected_constant") == 0) return _("expected a constant variable");
    else if (strcmp(err, "expected_type_at_arg") == 0) return _("provided unexpected variable type at argument[index]");
    else if (strcmp(err, "expected_type") == 0) return _("provided unexpected variable type");
    else if (strcmp(err, "expected_token") == 0) return _("expected a token");
    else if (strcmp(err, "expected_expression") == 0) return _("expected a expression in a if statement");
    else if (strcmp(err, "expected_identifier") == 0) return _("expected a identifier/literial");
    else if (strcmp(err, "unknown_symbol") == 0) return _("unknown symbol found when tokenize");
    return nullptr;
  }

  static const char* warning_info(const char* warn)
  {
    if (strcmp(warn, "wild_token") == 0) return "found a token with no context";
    return nullptr;
  }

  static const char* error_list()
  {
    return "\t[-Eundefined_variable]\n"
	   "\t[-Eundefined_reference]\n"
	   "\t[-Eundefined_function]\n"
	   "\t[-Ewild_identifier]\n"
	   "\t[-Etoo_few_args]\n"
	   "\t[-Etoo_many_args]\n"
	   "\t[-Eexpected_value]\n"
	   "\t[-Eexpected_constant]\n"
	   "\t[-Eexpected_type_at_arg]\n"
	   "\t[-Eexpected_type]\n"
	   "\t[-Eexpected_token]\n"
	   "\t[-Eexpected_expression]\n"
	   "\t[-Eexpected_identifier]\n"
	   "\t[-Eunknown_symbol]\n";
  }

  static const char* warning_list()
  {
    return "\t[-Wwild_token]\n";
  }

}

#endif
