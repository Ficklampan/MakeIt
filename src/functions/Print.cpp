#include "Functions.hpp"

static inline void print(MI::VariableRef* variable)
{
  if (variable->type == MI::VariableRef::STRING)
  {
    MI::VarString* str = VARIABLE_STRING(variable);
    size_t length = str->value.size();
    for (size_t i = 0; i < length; i++)
      putchar(str->value.at(i));
    printf("\n");
  }
  else if (variable->type == MI::VariableRef::INTEGER)
    printf("%i", VARIABLE_INTEGER(variable)->value);
  else if (variable->type == MI::VariableRef::LIST)
    printf("array[%lu]", VARIABLE_LIST(variable)->value.size());
}

int MI::function::exec_print(void* ptr, std::vector<VariableRef*> &args, char* &info)
{
  for (VariableRef* arg : args)
  {
    if (arg->type == VariableRef::LIST)
    {
      VarList* list = VARIABLE_LIST(arg);
      for (uint32_t i = 0; i < list->value.size(); i++)
      {
	VariableRef* variable = list->value.at(i);

	::print(variable);

	if (i < list->value.size() - 1)
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

