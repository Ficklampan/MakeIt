#include "Script.hpp"

#include "Token.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

#include "../Config.hpp"
#include "../System.hpp"

extern Config config;

int makeit::readScript(me::File* file, Storage* storage)
{
  void* data;
  uint32_t size;

  MIDEBUG(1, "reading data from file('%s')\n", file->getPath().c_str());
  if (!readFile(*file, data, size))
    return 0;
  std::string source((char*) data, size);

  std::vector<Token*> tokens;
  tokens.reserve(128);

  MIDEBUG(1, "making tokens\n");
  if (!Lexer::make(file, source, tokens, storage))
    return 0;

  storage->scripts.push_back(*file);

  me::BasicIterator<Token*> token_iter(tokens.data(), tokens.size());

  MIDEBUG(1, "parsing tokens[%lu]\n", tokens.size());
  if (!Parser::parse(token_iter, storage))
    return 0;

  return 1;
}
