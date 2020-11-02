#include "../Functions.hpp"

static inline void print(MI::Constant* constant)
{
  if (constant->type == MI::Constant::STRING)
    printf("%s", constant->value.s->c_str());
  else if (constant->type == MI::Constant::INTEGER)
    printf("%i", *constant->value.i);
  else if (constant->type == MI::Constant::LIST)
    printf("array[%lu]", constant->value.l->size());
}

int MI::function::print(void* ptr, std::vector<Constant*> &args)
{
  for (Constant* arg : args)
  {
    if (arg->type == Constant::LIST)
    {
      for (uint32_t i = 0; i < arg->value.l->size(); i++)
      {
	Constant* constant = arg->value.l->at(i);

	::print(constant);

	if (i < arg->value.s->size() - 1)
	  printf(" ");
      }
      printf("\n");
    }else
    {
      ::print(arg);
      printf("\n");
    }
  }
  return 1;
}

