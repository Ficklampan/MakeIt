#include "Script.hpp"

#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

#include "../Config.hpp"
#include "../System.hpp"

#include <cstdarg>

extern makeit::Config config;

int makeit::read_script(me::File* file, Storage* storage)
{
  void* data;
  uint32_t size;

  MIDEBUG(1, "reading data from file('%s')\n", file->getPath().c_str());
  if (!readFile(*file, data, size))
    return 0;
  std::string source((char*) data, size);

  std::vector<Token*> tokens;
  tokens.reserve(128);

  MIDEBUG(1, "making tokens\n");
  try {
    Lexer::tokenize_tokens(file, source, tokens, storage, 0);
  }catch (const Exception<TokenLocation> &e)
  {
    print_error(e.get_location(), e.get_message());
  }

  storage->scripts.push_back(*file);

  me::BasicIterator<Token*> token_iter(tokens.data(), tokens.size());

  MIDEBUG(1, "parsing tokens[%lu]\n", tokens.size());
  try {
    Parser::parse_tokens(token_iter, storage, 0);
  }catch (const Exception<TokenLocation> &e)
  {
    print_error(e.get_location(), e.get_message());
  };
  return 1;
}

static inline void printLocation(const makeit::TokenLocation &loc)
{
  uint32_t end = loc.source->size();
  for (uint32_t i = loc.pos; i < loc.source->size(); i++)
  {
    if (loc.source->at(i) == '\n')
    {
      end = i - loc.pos;
      break;
    }
  }

  if (end - loc.first < 1 || loc.pos - loc.first < 1)
  {
    return;
  }

  std::string marked(&loc.source->c_str()[loc.first], loc.pos - loc.first);
  std::string line = "\e[4m" + marked + "\e[0m" + std::string(&loc.source->c_str()[loc.pos], end);

  /* printing the stuff */
  printf("%s[%u:%u]: '%s'\n", loc.file->getPath().c_str(), loc.line, loc.column, line.c_str());
}

void makeit::print_error(const TokenLocation &loc, const char* format, ...)
{
  printLocation(loc);
  va_list args;
  va_start(args, format);
  error(format, args);
  va_end(args);
}

void makeit::print_warning(const TokenLocation &loc, const char* format, ...)
{
  printLocation(loc);
  va_list args;
  va_start(args, format);
  warning(format, args);
  va_end(args);
}
