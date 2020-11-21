#ifndef MAKEIT_HPP
  #define MAKEIT_HPP

#define NAME "MakeIt"

namespace makeit {

  void usage(const char* arg);
  void setupLocale();

  int main(int argc, char** argv);
  int parseArgs(int argc, char** argv);
  int init();

}

#endif
