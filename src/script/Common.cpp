#include "Common.hpp"

#include <cstring>
#include <cstdarg>

static inline void printLocation(MI::Token::Location* loc)
{
  uint32_t end = loc->source->size();
  for (uint32_t i = loc->pos; i < loc->source->size(); i++)
  {
    if (loc->source->at(i) == '\n')
    {
      end = i - loc->pos;
      break;
    }
  }

  if (end - loc->first < 1 || loc->pos - loc->first < 1)
  {
    return;
  }

  std::string marked(&loc->source->c_str()[loc->first], loc->pos - loc->first);
  std::string line = "\e[4m" + marked + "\e[0m" + std::string(&loc->source->c_str()[loc->pos], end);

  /* printing the stuff */
  printf("At [%u:%u]: '%s'\n", loc->line, loc->column, line.c_str());
}

void MI::printError(Token::Location* loc, const char* format, ...)
{
  printLocation(loc);
  va_list args;
  va_start(args, format);
  printf("\e[31m:: ");
  vprintf(format, args);
  printf("\e[0m\n");
  va_end(args);
}

void MI::printWarning(Token::Location* loc, const char* format, ...)
{
  printLocation(loc);
  va_list args;
  va_start(args, format);
  printf("\e[33m:: ");
  vprintf(format, args);
  printf("\e[0m\n");
  va_end(args);
}
