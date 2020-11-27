#include "../Functions.hpp"

#include "../FuncUtils.hpp"

static inline void print(makeit::Variable* variable, makeit::Storage* storage)
{
  if (variable->type == makeit::Variable::LIST)
    printf("array[%lu]", variable->as_list()->size());
  else if (variable->type == makeit::Variable::STRING)
    printf("%s", variable->as_string()->c_str());
}

makeit::Function* makeit::function::make_print()
{
  return new Function({
      new Argument(Variable::STRING, Argument::ENDLESS)
      }, exec_print);
}

int makeit::function::exec_print(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  for (Variable* arg : args)
  {
    if (arg->type == Variable::LIST)
    {
      std::vector<Variable*>* list = arg->as_list();
      for (uint32_t i = 0; i < list->size(); i++)
      {
	Variable* variable = list->at(i);

	::print(variable, storage);

	if (i < list->size() - 1)
	  printf(" ");
      }
      printf("\n");
    }else
    {
      ::print(arg, storage);
      printf("\n");
    }
  }
  return 1;
}
