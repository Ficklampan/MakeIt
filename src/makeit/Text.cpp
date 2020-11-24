#include "Text.hpp"

/* errors */
static const char* Eundefined_variable = "undefined variable '%s' [-Eundefined_variable]";
static const char* Eundefined_reference = "undefined reference '%s' [-Eundefined_reference]";
static const char* Eundefined_function = "undefined function '%s' [-Eundefined_function]";
static const char* Etoo_few_args = "too few arguments [-Etoo_few_args]";
static const char* Etoo_many_args = "too many arguments [-Etoo_many_args]";
static const char* Eexpected_value = "expected value [-Eexpected_value]";
static const char* Eexpected_constant = "expected constant [-Eexpected_constant]";
static const char* Eexpected_token = "expected token '%s' [-Eexpected_token]";
static const char* Eexpected_type_at_arg = "expected type '%s' at arg[%u] but found type '%s' [-Eexpected_type_at_arg]";
static const char* Eexpected_type_at_member = "expected type '%s' at member[%s] but found type '%s' [-Eexpected_type]";
static const char* Eexpected_value_type = "expected type '%s' but found type '%s' [-Eexpected_type]";
static const char* Eexpected_token_type = "expected token type '%s' [-Eexpected_token]";
static const char* Eexpected_expression = "expected expression [-Eexpected_expression]";
static const char* Eexpected_identifier = "expected identifier [-Eexpected_identifier]";
static const char* Eunknown_symbol = "unknown symbol '%c' [-Eunknown_symbol]";
static const char* Emakefile = "failed to generate Makefile '%s' [-Emakefile]";

/* warnings */
static const char* Wwild_token = "found token in the wild [-Wwild_token]";


const char* makeit::get_error(const Error err)
{
  switch (err)
  {
    case EUNDEFINED_VARIABLE: return Eundefined_variable;
    case EUNDEFINED_REFERENCE: return Eundefined_reference;
    case EUNDEFINED_FUNCTION: return Eundefined_function;
    case ETOO_FEW_ARGS: return Etoo_few_args;
    case ETOO_MANY_ARGS: return Etoo_many_args;
    case EEXPECTED_VALUE: return Eexpected_value;
    case EEXPECTED_CONSTANT: return Eexpected_constant;
    case EEXPECTED_TOKEN: return Eexpected_token;
    case EEXPECTED_TYPE_AT_ARG: return Eexpected_type_at_arg;
    case EEXPECTED_TYPE_AT_MEMBER: return Eexpected_type_at_member;
    case EEXPECTED_VALUE_TYPE: return Eexpected_value_type;
    case EEXPECTED_TOKEN_TYPE: return Eexpected_token_type;
    case EEXPECTED_EXPRESSION: return Eexpected_expression;
    case EEXPECTED_IDENTIFIER: return Eexpected_identifier;
    case EUNKNOWN_SYMBOL: return Eunknown_symbol;
    case EMAKEFILE: return Emakefile;
    case ELAST: return nullptr;
  }
  return nullptr;
}

const char* makeit::get_error_id(const Error err)
{
#define ERROR_ID(t) { if (t == err) return #t; }

  ERROR_ID(EUNDEFINED_VARIABLE);
  ERROR_ID(EUNDEFINED_REFERENCE);
  ERROR_ID(EUNDEFINED_FUNCTION);
  ERROR_ID(ETOO_FEW_ARGS);
  ERROR_ID(ETOO_MANY_ARGS);
  ERROR_ID(EEXPECTED_VALUE);
  ERROR_ID(EEXPECTED_CONSTANT);
  ERROR_ID(EEXPECTED_TOKEN);
  ERROR_ID(EEXPECTED_TYPE_AT_ARG);
  ERROR_ID(EEXPECTED_TYPE_AT_MEMBER);
  ERROR_ID(EEXPECTED_VALUE_TYPE);
  ERROR_ID(EEXPECTED_TOKEN_TYPE);
  ERROR_ID(EEXPECTED_EXPRESSION);
  ERROR_ID(EEXPECTED_IDENTIFIER);
  ERROR_ID(EUNKNOWN_SYMBOL);
  ERROR_ID(EMAKEFILE);
  return nullptr;

#undef ERROR_ID
}

const char* makeit::get_warning(const Warning warn)
{
  switch (warn)
  {
    case WWILD_TOKEN: return Wwild_token;
  }
  return nullptr;
}

const char* makeit::get_warning_id(const Warning warn)
{
#define WARNING_ID(t) { if (t == warn) return #t; }

  WARNING_ID(WWILD_TOKEN);
  return nullptr;

#undef WARNING_ID
}
