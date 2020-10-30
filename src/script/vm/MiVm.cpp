#include "MiVm.hpp"

/*
 * NOTE
 * - All variables created are global
 */

mi::VM::VM(Storage* storage, std::vector<Instruction*>* instructions)
{
  this->storage = storage;
  this->instructions = instructions;
}

int mi::VM::clock()
{
  if (state.count >= instructions->size())
    return 0;

  Instruction* instr = instructions->at(state.count++);

  return execute(instr);
}

static inline int VM_ASSIGN_VARIABLE(mi::Variable* &a, mi::Variable* &b, int mode)
{
  if (mode == 1 || mode == 2)
  {
    /* if a is array */
    if (a->getFlag(mi::Variable::ARRAY))
    {
      /* clear the value if assign */
      if (mode == 1)
	a->getValue().a->clear();

      /* append B array value to A array value */
      if (b->getFlag(mi::Variable::ARRAY))
      {
	for (void* v : *b->getValue().a)
	  a->getValue().a->push_back(v);

      /* append B value to A array value */
      }else
	a->getValue().a->push_back(b->getValue().v);

    /* if a is not array */
    }else
    {
      /* if assign */
      if (mode == 1)
      {
	/* NOTE: add more here if theres more than ARRAY flag. constant flag is ignored */
	a->setFlag(mi::Variable::ARRAY, b->getFlag(mi::Variable::ARRAY));

	a->setValue(b->getValue().v);

      /* if append */
      }else
      {
	printf(":: can't append unknown data type.\n");
	return 0;
      }
    }
  }
  return 1;
}

/* ? Multiple States */
int mi::VM::execute(Instruction* instr)
{
  /* OPCODE::ASSIGN*/
  if (instr->code == OPCODE_ASSIGN)
    state.setFlag(FLAG_ASSIGN, true);
  
  /* OPCODE::APPEND */
  else if (instr->code == OPCODE_APPEND)
    state.setFlag(FLAG_APPEND, true);

  /* OPCODE::CONSTANT */
  else if (instr->code == OPCODE_CONSTANT)
  {
    Variable* v = (Variable*) instr->value;

    if (state.getFlag(FLAG_ASSIGN))
      VM_ASSIGN_VARIABLE(state.v, v, 1);
    else if (state.getFlag(FLAG_APPEND))
      VM_ASSIGN_VARIABLE(state.v, v, 2);
    else
    {
      printf(":: nothing to do with constant value.\n");
      return 0;
    }

  /* OPCODE::VARIABLE */
  }else if (instr->code == OPCODE_VARIABLE)
  {
    std::string* var_name = (std::string*) instr->value;

    Variable* v = storage->variables[*var_name];

    // TODO: error if var not found

    if (state.getFlag(FLAG_ASSIGN))
      VM_ASSIGN_VARIABLE(state.v, v, 1);
    else if (state.getFlag(FLAG_APPEND))
      VM_ASSIGN_VARIABLE(state.v, v, 2);
    else
    {
      state.v = new Variable(nullptr, 0);
      storage->variables[*var_name] = state.v;
    }

  /* OPCODE::ARG */
  }else if (instr->code == OPCODE_ARG)
  {
    printf("==> the value: <%s>\n", state.v->getValue().v);
    state.args.push_back(state.v);
    state.v = new Variable(nullptr, Variable::UNUSED);

  /* OPCODE::PREPARE_CALL */
  }else if (instr->code == OPCODE_PREPARE_CALL)
  {
    std::string* func_name = (std::string*) instr->value;

    state.f = storage->functions[*func_name];

    if (state.f == nullptr)
    {
      printf(":: undefined function '%s'.\n", func_name->c_str());
      return 0;
    }

    state.setFlag(FLAG_ARG, true);
    state.setFlag(FLAG_ASSIGN, true);


  /* OPCODE::CALL */
  }else if (instr->code == OPCODE_CALL)
  {
    if (!state.f->exec(state.args))
      return 0;

    state.args.clear();


  /* OPCODE::RESET */
  }else if (instr->code == OPCODE_RESET)
  {
    state.status = 0;
    state.v = new Variable(nullptr, Variable::UNUSED);
    state.f = nullptr;
  }
  return 1;
}

bool mi::VM::State::getFlag(Flag flag)
{
  return (status & flag) > 0;
}

void mi::VM::State::setFlag(Flag flag, bool val)
{
  if (val)
    status |= flag;
  else
    status &= (~flag);
}
