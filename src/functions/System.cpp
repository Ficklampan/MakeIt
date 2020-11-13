#include "../Functions.hpp"

MI::function::System::System()
{
  desc = "";
  args = ArgBuilder().type({MI::Constant::STRING, MI::Constant::LIST}).endless();
}

int MI::function::System::execute(void* ptr, std::vector<Constant*> &args, char* &info)
{
  return 1;
}
