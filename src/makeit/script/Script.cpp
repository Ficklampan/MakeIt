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

  MIDEBUG(1, "reading data from file('%s')\n", file->get_path().c_str());
  if (!readFile(*file, data, size))
    return 0;
  std::string source((char*) data, size);

  std::vector<Token*> tokens;
  tokens.reserve(128);

  MIDEBUG(1, "making tokens\n");
  try {
    Lexer::tokenize_tokens(file, source, tokens, storage, 0);
  }catch (const Exception<TokenLocation*> &e)
  {
    print_error(e.get_location(), e.get_message());
  }

  storage->scripts.push_back(*file);

  me::Iterator<Token*> token_iter(tokens.data(), tokens.size());

  MIDEBUG(1, "parsing tokens[%lu]\n", tokens.size());
  try {
    Parser::parse_tokens(token_iter, storage, 0);
  }catch (const Exception<TokenLocation*> &e)
  {
    print_error(e.get_location(), e.get_message());
  };
  return 1;
}

static inline void printLocation(const makeit::TokenLocation &loc)
{
  std::string str;
  uint32_t line = loc.begin_line;
  bool marked = false;
  for (uint32_t i = loc.begin; i < loc.source->size(); i++)
  {
    char c = loc.source->at(i);

    if (i == loc.size)
    {
      str.append("\e[0m");
      marked = false;
    }

    if (c == '\n')
    {
      printf(" %u | %s\e[0m\n", line, str.c_str());
      str.clear();
      if (marked)
	str.append("\e[4m");
      line++;
      if (i >= loc.pos + loc.size)
	break;
    }

    if (i == loc.pos)
    {
      str.append("\e[4m");
      marked = true;
    }

    if (c != '\n')
      str += c;
  }
}

void makeit::print_error(const TokenLocation* loc, const char* format, ...)
{
  if (loc != nullptr)
    printf("\e[97m%s:%u:%u:\e[0m ", loc->file->get_path().c_str(), loc->line, loc->column);

  va_list args;
  va_start(args, format);
  error(format, args);
  va_end(args);

  if (loc != nullptr)
    printLocation(*loc);
}

void makeit::print_warning(const TokenLocation* loc, const char* format, ...)
{
  if (loc != nullptr)
    printf("\e[97m%s:%u:%u:\e[0m ", loc->file->get_path().c_str(), loc->line, loc->column);

  va_list args;
  va_start(args, format);
  warning(format, args);
  va_end(args);

  if (loc != nullptr)
    printLocation(*loc);
}
