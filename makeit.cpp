#include <iostream>

#include "makeit.h"
#include "makeit_parser.h"

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "usage: 'makeit <directory>'\n";
    return 0;
  }
  std::string directory = argv[1];
  std::cout << "making { " << directory << " }\n";
  makeit::parse(nullptr, directory);
  std::cout << "making complete!\n";
  return 0;
}
