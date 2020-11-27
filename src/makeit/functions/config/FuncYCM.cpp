#include "../Functions.hpp"

#include "../FuncUtils.hpp"

makeit::Function* makeit::function::make_YCM()
{
  return new Function({
      }, exec_ycm);
}
int makeit::function::exec_ycm(void* ptr, std::vector<Variable*> &args)
{
  return 1;
}
