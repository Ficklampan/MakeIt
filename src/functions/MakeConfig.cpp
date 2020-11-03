#include "../Functions.hpp"

/* TODO: make so you can put a string and not only a list */
int MI::function::makefile(void* ptr, std::vector<Constant*> &args, char* &info)
{
  if (args.size() != 2)
  {
    info = (char*) (args.size() > 2 ? "too many arguments" : "too few arguments");
    return 0;
  }
  return 1;
}
