#include <iostream>

#include "makeit.hpp"
#include "makeit_parser.hpp"

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "usage: 'makeit <directory>'\n";
    return 0;
  }
  std::string directory = argv[1];
  std::cout << "making { " << directory << " }\n";
  makeit::Parser parser;
  parser.init();
  int result = parser.parse(nullptr, directory);
  if (result != 1)
  {
    std::cout << "MakeIt failed, errors occurred\n";
    return 0;
  }
  std::cout << "MakeIt Survived!\n";
  return 0;
}
