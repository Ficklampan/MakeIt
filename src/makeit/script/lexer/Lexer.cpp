#include "Lexer.hpp"
#include "../../Text.hpp"
#include "../Script.hpp"

#include "../../Config.hpp"

extern makeit::Config config;

int makeit::Lexer::tokenize_tokens(me::File* file, std::string &source, std::vector<Token*> &tokens, Storage* storage, uint8_t flags)
{
  Iterator iterator(file, &source);

  while (iterator.hasNext())
  {
    Token* token = nullptr;
    size_t index = source.rfind("\n", iterator.index);
    iterator.begin = index == std::string::npos ? 0 : index + 1;
    iterator.begin_line = iterator.line;
    tokenize_token(iterator, token, storage, flags);

    if (token != nullptr)
      tokens.push_back(token);
  }
  return 1;
}

int makeit::Lexer::tokenize_token(Iterator &source, Token* &token, Storage* storage, uint8_t flags)
{
  source.pos = source.index;
  source.start_line = source.line;
  source.start_column = source.column;

  char c = source.next();

  /* Token::LITERIAL / Token::CALL / Token::FOREACH / Token::IF / Token::ELSE / Token::END / Token::CONSTANT BOOLEAN */
  if (LEXER_IS_NAME(c))
  {
    tokenize_literial(c, source, token, storage, flags);
    return 1;

  /* Token::CONSTANT */
  }else if (c == '"' || (LEXER_IS_DIGIT(c) || (c == '-' && LEXER_IS_DIGIT(source.peek()))) || c == '[' || c == '{' || c == '$')
  {
    tokenize_constant(c, source, token, storage, flags);
    return 1;

  /* Token::PUNCTUATOR */
  }else if (LEXER_IS_PUNCTUATOR(c))
  {
    tokenize_punctuator(c, source, token, storage, flags);
    return 1;

  }else if (LEXER_IS_WILDCARD(c))
  {
    tokenize_wildcard(c, source, token, storage, flags);
    return 1;



  /* Token::BREAK */
  }else if ((flags & NO_BREAK) == 0 && (c == '\n' || c == ';'))
  {
    MIDEBUG(2, "[Lexer] > next token is BREAK\n");

    LEXER_NEW_TOKEN(Token::BREAK, nullptr);

    MIDEBUG(2, "[Lexer] > BREAK token created\n");
    return 1;

  /* Comment */
  }else if (c == '#')
  {
    while (source.hasNext())
    {
      if (source.next() == '\n')
        break;
    }
    return 1;

  /* Error */
  }else if ((flags & NO_ERROR) == 0 && !LEXER_IS_EMPTY(c))
  {
    throw Exception(new TokenLocation(source), EUNKNOWN_SYMBOL, { c });
  }
  return 1;
}


const bool makeit::Lexer::Iterator::hasNext() const
{
  return index < source->size();
}

char& makeit::Lexer::Iterator::peek() const
{
  return source->at(index);
}

char& makeit::Lexer::Iterator::next()
{
  if (source->at(index) == '\n')
  {
    line++;
    column = 0;
  }else
    column++;
  return source->at(index++);
}


