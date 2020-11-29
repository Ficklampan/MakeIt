#include "Parser.hpp"
#include "../../Text.hpp"
#include "../Script.hpp"

#include "../../Config.hpp"

int makeit::Parser::parse_literial(Token* token, me::Iterator<Token*> &tokens, Storage* storage, uint8_t flags)
{
  std::string* name = token->value.s;


  if (tokens.peek()->type == Token::PUNCTUATOR)
  {
    token = tokens.next();
    Variable* variable_a = storage->variables[*name];
    
    /* punctuator type */
    int p = *token->value.i;

    token = tokens.next();

    /* Get next Constant / Variable */
    Variable* variable_b = nullptr;
    if (!get_variable(token, tokens, storage, variable_b, flags))
      return 0;

    if (p == Token::EQUAL)
    {
      if (variable_a == nullptr)
      {
        variable_a = new Variable(&token->location, variable_b->type, nullptr);
        storage->variables[*name] = variable_a;
      }else
        variable_a->type = variable_b->type;
      variable_a->value = variable_b->value;
    }

    /* [Error] undefined variable */
    if (variable_a == nullptr)
    {
      throw Exception(&token->location, EUNDEFINED_VARIABLE, { name->c_str() });
    }else if (p == Token::PLUS_EQUAL)
      (*variable_a) += variable_b;
  }
  return 1;
}
