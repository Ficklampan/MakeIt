#include "../Functions.hpp"

MI::function::YCM::YCM()
{
  desc = "";
  args = ArgBuilder().build();
}

int MI::function::YCM::execute(void* ptr, std::vector<Constant*> &args, char* &info)
{
  return 1;
}
