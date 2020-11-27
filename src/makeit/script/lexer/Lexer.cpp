#include "Lexer.hpp"
#include "../../Text.hpp"
#include "../Script.hpp"

#include "../../Config.hpp"

extern makeit::Config config;

int makeit::Lexer::tokenize_tokens(me::File* file, std::string &source, std::vector<Token*> &tokens, Storage* storage, uint8_t flags)
{
  TokenLocation location = { file, &source, 0, 0, 0, 0, };

  me::Iterator<char> source_iter((char*) source.data(), source.size(), [](void* ptr, char &c, uint32_t &i) {
      TokenLocation* location = (TokenLocation*) ptr;
      if (c == '\n')
      {
	location->column = 0;
	location->line++;
      }else
	location->column++;
      location->pos++;
  }, &location);

  while (source_iter.hasNext())
  {
    Token* token = nullptr;
    if (!tokenize_token(source_iter, location, token, storage, flags))
      return 0;

    if (token != nullptr)
      tokens.push_back(token);
  }
  return 1;
}

int makeit::Lexer::tokenize_token(me::Iterator<char> &source, TokenLocation &location, Token* &token, Storage* storage, uint8_t flags)
{
  location.first = location.pos;
  char c = source.next();

  /* Token::LITERIAL / Token::CALL / Token::IF / Token::ELSE / Token::END / Token::CONSTANT BOOLEAN */
  if (LEXER_IS_NAME(c))
  {
    if (!tokenize_literial(c, source, location, token, storage, flags))
      return 0;
    return 1;

  /* Token::CONSTANT */
  }else if (c == '"' || (LEXER_IS_DIGIT(c) || (c == '-' && LEXER_IS_DIGIT(source.peek()))) || c == '[' || c == '{' || c == '$')
  {
    if (!tokenize_constant(c, source, location, token, storage, flags))
      return 0;
    return 1;

  /* Token::PUNCTUATOR */
  }else if (LEXER_IS_PUNCTUATOR(c))
  {
    if (!tokenize_punctuator(c, source, location, token, storage, flags))
      return 0;
    return 1;

  }else if (LEXER_IS_WILDCARD(c))
  {
    if (!tokenize_wildcard(c, source, location, token, storage, flags))
      return 0;
    return 1;



  /* Token::BREAK */
  }else if ((flags & NO_BREAK) == 0 && (c == '\n' || c == ';'))
  {
    MIDEBUG(2, "[Lexer] > next token is BREAK\n");

    token = new Token(Token::BREAK, nullptr, TokenLocation(location));

    MIDEBUG(2, "[Lexer] > BREAK token created\n");
    return 1;

  /* New Line / Comment */
  }else if (c == '\\' || c == '#')
  {
    bool comment = c == '#';

    while (source.hasNext())
    {
      c = source.next();
      
      if (c == '\n')
      {
        break;
      }
    }
    return 1;

  /* Error */
  }else if (!LEXER_IS_EMPTY(c))
  {
    throw Exception(location, EUNKNOWN_SYMBOL, { c });
  }
  return 1;
}
