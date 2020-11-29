#include "Makefile.hpp"

#include "Util.hpp"

#include <algorithm>

using namespace makeit::gnu_make;

Makefile::Makefile()
{
}

void Makefile::put_element(Element* element)
{
  elements.push_back(element);
}

int Makefile::write(std::string &str)
{
  int pos = 0;
  for (Element* element : elements)
  {
    if (element->get_pos() == -2)
      element->set_pos(pos++);
  }

  std::sort(elements.begin(), elements.end(), [](const Element* e1, const Element* e2) {
      return e2->get_pos() > e1->get_pos();
      });

  for (const Element* element : elements)
  {
    if (element->get_type() == Element::VARIABLE)
      write_variable(str, (Variable*) element);
    else if (element->get_type() == Element::RULE)
      write_rule(str, (Rule*) element);
    else if (element->get_type() == Element::TEXT)
      write_text(str, (Text*) element);
  }
  return 1;
}

void Makefile::write_variable(std::string &str, const Variable* var)
{
  str.append(var->get_name()).append(" = ");
  const std::vector<std::string> &value = var->get_value();
  for (uint32_t i = 0; i < value.size(); i++)
  {
    std::string element = value.at(i);
    makeit::string_replace(element, " ", "\\ ");
    makeit::string_replace(element, "\"", "\\\"");
    str.append(element);
    if (i < value.size() - 1)
      str.append(value.size() > 4 ? " \\\n\t" : " ");
  }
  str += '\n';
}

void Makefile::write_rule(std::string &str, const Rule* rule)
{
  if (rule->is_phony())
    str.append(".PHONY: ").append(rule->get_target()) += '\n';
  str.append(rule->get_target()).append(rule->get_prerequisites().empty() ? ":\n" : ": " + rule->get_prerequisites() + "\n");
  for (const Command &command : rule->get_commands())
    str.append("\t").append(command.args) += '\n';
}

void Makefile::write_text(std::string &str, const Text* text)
{
  for (const std::string &line : text->get_lines())
  {
    if (text->is_comment())
      str.append("# ");
    str.append(line) += '\n';
  }
}
