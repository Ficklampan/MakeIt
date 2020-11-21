#include "Parser.hpp"
#include "../../Text.hpp"
#include "../Script.hpp"

int makeit::Parser::parse_literial(Token* token, me::BasicIterator<Token*> &tokens, Storage* storage, uint8_t flags)
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
        variable_a = new Variable(variable_b->type, nullptr);
        storage->variables[*name] = variable_a;
      }else
        variable_a->type = variable_b->type;
      variable_a->value = variable_b->value;
    }

    /* [Error] undefined variable */
    if (variable_a == nullptr)
    {
      printError(token->location, Eundefined_variable, name->c_str());
      return 0;
    }else if (p == Token::PLUS_EQUAL)
      (*variable_a) += variable_b;

  /* [Error] found identifier in the wild */
  }else
  {
    printWarning(token->location, Ewild_identifier);
  }
  return 1;
}
