#include "Functions.hpp"

makeit::Function* makeit::function::make_YCM()
{
  return new Function(0,
      new uint16_t[0]{
      }, exec_ycm);
}
int makeit::function::exec_ycm(void* ptr, std::vector<Variable*> &args, char* &info)
{
  return 1;
}
