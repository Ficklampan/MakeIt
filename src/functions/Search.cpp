#include "../Functions.hpp"

MI::function::Search::Search()
{
  desc = "";
  args = ArgBuilder()
    .type({MI::Constant::STRING, MI::Constant::LIST})
    .type({MI::Constant::STRING, MI::Constant::LIST})
    .type(MI::Constant::BOOLEAN)
    .type(MI::Constant::LIST)
    .build();
}

int MI::function::Search::execute(void* ptr, std::vector<Constant*> &args, char* &info)
{
  return 1;
}
