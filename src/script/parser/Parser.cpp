#include "Parser.hpp"

mi::Parser::Parser(me::Iterator<Token*>* tokens)
{
  this->tokens = tokens;
}

std::vector<mi::Instruction*>& mi::Parser::getInstructions()
{
  return instructions;
}

int mi::Parser::parseTokens()
{
  while (tokens->hasNext())
  {
    if (!parseTokens(instructions))
      return 0;
  }
  return 1;
}

int mi::Parser::parseTokens(std::vector<Instruction*> &instructions)
{
  while (tokens->hasNext())
  {
    if (!parseToken(instructions))
      return 0;
  }

  return 1;
}

int mi::Parser::parseToken(std::vector<Instruction*> &instructions)
{
  Token* token = tokens->next();

  /* IDENTIFIER */
  if (token->type == Token::IDENTIFIER)
  {
    /* check if function call */
    if (tokens->hasNext() && tokens->peek()->type == Token::PUNCTUATOR && *tokens->peek()->value.i == L_PAREN)
    {
      tokens->next();

      std::string* func_name = (std::string*) token->value.v;

      instructions.push_back(new Instruction(OPCODE_PREPARE_CALL, func_name));

      bool extra_arg = false;

      while (tokens->hasNext())
      {
        token = tokens->peek();

        if (token->type == Token::PUNCTUATOR)
        {
          int p = *token->value.i;

          if (p == R_PAREN) // end of function
          {
            tokens->next();
            break;
          }else if (p == COMMA)
          {
            tokens->next();
            instructions.push_back(new Instruction(OPCODE_ARG, nullptr));
          }
        }else if (token->type == Token::RESET)
        {
          printf(":: ';' not allowed here.\n");
          return 0;
        }

        extra_arg = true;

        if (!parseToken(instructions))
          return 0;
      }

      if (extra_arg)
        instructions.push_back(new Instruction(OPCODE_ARG, nullptr));

      instructions.push_back(new Instruction(OPCODE_CALL, nullptr));

    /* variable */
    }else
      instructions.push_back(new Instruction(OPCODE_VARIABLE, token->value.v));

  /* STRING */
  }else if (token->type == Token::STRING)
  {
    instructions.push_back(new Instruction(OPCODE_CONSTANT, new Variable(token->value.v, Variable::CONSTANT)));

  /* PUNCTUATOR */
  }else if (token->type == Token::PUNCTUATOR)
  {
    int p = *token->value.i;

    if (p == EQUAL)
      instructions.push_back(new Instruction(OPCODE_ASSIGN, nullptr));
    else if (p == PLUS_EQUAL)
      instructions.push_back(new Instruction(OPCODE_APPEND, nullptr));

  /* RESET */
  }else if (token->type == Token::RESET)
  {
    instructions.push_back(new Instruction(OPCODE_RESET, nullptr));
  }

  return 1;
}
