#ifndef TEXTS_H
  #define TEXTS_H

static const char* ERR_TOO_FEW_ARGS = ":: too few arguments. expected %s, found %i, at `%s`.\n";
static const char* ERR_TOO_MANY_ARGS = ":: too many arguments. expected %s, found %i, at `%s`.\n";
static const char* ERR_NO_DIR_SPEC = ":: not directory specified, at `%s`.\n";
static const char* ERR_NO_VAR_NAME_SPECIFIED = ":: no variable name specified, at %s.\n";
static const char* ERR_VAR_NOT_FOUND = ":: variable[%s] not found, at `%s`.\n";
static const char* ERR_MAKEFILE_FAILED = ":: failed to construct makefile: `%s`.\n";
static const char* ERR_FAILED_PARSING = ":: failed to parse file `%s`.\n";
static const char* ERR_DEPS_DIR_NOT_SPECIFIED = ":: dependency directory not specified, at %s.\n";
static const char* ERR_UNKNOWN = ":: unknown %s '%s', at %s.\n";

static const char* WARN_SEARCH_NOT_FOUND = "!! no files was found with pattern '%s'.\n";

static const char* INFO_CONSTRUCT_MAKEFILE = "==> [...] constructing makefile\n";
static const char* INFO_CONSTRUCT_MAKEFILE_DONE = "==> [complete] makefile constructed\n";

#endif
