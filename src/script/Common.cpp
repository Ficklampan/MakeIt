#include "Common.hpp"

#include <cstring>

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

void MI::printError(char* str, Token::Location* loc)
{
  printLocation(loc);
  printf("\e[31m:: %s\e[0m\n", str);
}

void MI::printWarning(char* str, Token::Location* loc)
{
  printLocation(loc);
  printf("\e[33m:: %s\e[0m\n", str);
}
