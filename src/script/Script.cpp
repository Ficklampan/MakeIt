#include "Script.hpp"

#include "Token.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

#include "../System.hpp"

int MI::readScript(me::File* file, Storage* storage)
{
  void* data;
  uint32_t size;

  if (!readFile(*file, data, size))
    return 0;
  std::string source((char*) data, size);

  std::vector<Token*> tokens;
  tokens.reserve(256);

  if (!Lexer::make(file, source, tokens))
    return 0;

  storage->scripts.push_back(*file);

  me::BasicIterator<Token*> token_iter(tokens.data(), tokens.size());
  if (!Parser::parse(token_iter, storage))
    return 0;

  return 1;
}
