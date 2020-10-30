#include "../Functions.hpp"

int mi::function::print(std::vector<Variable*> &variables)
{
  for (Variable* arg : variables)
  {
    if (arg->getFlag(Variable::UNUSED))
    {
      printf("(nul)\n");
    }else if (arg->getFlag(Variable::ARRAY))
    {
      for (void* v : *arg->getValue().a)
	printf("%s\n", (char*) v);
    }else
      printf("%s\n", (char*) arg->getValue().v);
  }
  return 1;
}

