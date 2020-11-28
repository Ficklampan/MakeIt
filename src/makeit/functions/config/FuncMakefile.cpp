#include "../Functions.hpp"

#include "../FuncUtils.hpp"

#include "../../configs/make/Makefile.hpp"

#include "../../Project.hpp"
#include "../../System.hpp"

static void GENERATE_MAKEFILE(makeit::gnu_make::Makefile &makefile, makeit::Project &project, std::vector<makeit::gnu_make::Element*> &elements);

makeit::Function* makeit::function::make_makefile()
{
  return new Function(
      {
      new Argument(Variable::STRING),
      new StructArg({
	  StructArg::Arg("pos", false, new Argument(Variable::INTEGER)),
	  StructArg::Arg("lines", true, new ListArg(new Argument(Variable::STRING)))
	  }, Argument::ENDLESS)
      }, exec_makefile);
}

int makeit::function::exec_makefile(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  me::File file = *args.at(0)->as_string();

  std::vector<gnu_make::Element*> elements;
  for (uint32_t i = 1; i < args.size(); i++)
  {
    Variable::v_struct &st = *args.at(i)->as_struct();

    Variable* st_value = st["lines"];
    int pos = st.find("pos") == st.end() ? -2 : *st["pos"]->as_integer();

    std::vector<std::string> lines;
    GET_STRINGS(st_value, lines);
    elements.push_back(new gnu_make::Text(lines, false, pos));
  }

  gnu_make::Makefile makefile;
  GENERATE_MAKEFILE(makefile, *project, elements);

  std::string makefile_data;
  makefile_data.reserve(4096);
  makefile.write(makefile_data);

  makeit::writeFile(file, makefile_data.data(), makefile_data.size());
  return 1;
}

static void GENERATE_MAKEFILE(makeit::gnu_make::Makefile &makefile, makeit::Project &project, std::vector<makeit::gnu_make::Element*> &elements)
{
  using namespace makeit::gnu_make;

  makefile.put_element(new Variable("NAME", project.name));
  makefile.put_element(new Variable("CC", project.config.cc));
  makefile.put_element(new Variable("BUILD", project.config.build.getPath()));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* flags */
  makefile.put_element(new Variable("FLAGS", project.config.flags));
  /* libraries */
  makefile.put_element(new Variable("LIBS", project.config.libraries));
  /* includes */
  makefile.put_element(new Variable("INCS", project.config.includes));
  /* library paths */
  makefile.put_element(new Variable("LPATHS", project.config.library_paths));
  /* include paths */
  makefile.put_element(new Variable("IPATHS", project.config.include_paths));

  /* definitions */
  std::vector<std::string> defs;
  for (auto &[key, value] : project.config.definitions)
  {
    std::string def = "-D" + key;
    if (!value.empty())
      def.append("=").append(value);
    defs.push_back(def);
  }
  makefile.put_element(new Variable("DEFS", defs));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* compiler options */
  makefile.put_element(new Variable("COPTS", "$(FLAGS) $(INCS) $(IPATHS) $(DEFS)"));
  /* linker options */
  makefile.put_element(new Variable("LOPTS", "$(FLAGS) $(LIBS) $(LPATHS)"));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* source files */
  makefile.put_element(new Variable("SOURCES", project.config.sources));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* object files */
  makefile.put_element(new Variable("OBJECTS", "$(SOURCES:%=$(BUILD)/%.o)"));
  /* dependency files */
  makefile.put_element(new Variable("DEPENDS", "$(OBJECTS:%.o=%.d)"));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* linking rule */
  makefile.put_element(new Rule("$(NAME)", "$(OBJECTS)", {
	Command("@$(CC) -o $@ $^ $(LOPTS)")
	}, 0));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* compiling rule */
  makefile.put_element(new Rule("$(BUILD)/%.o", "%", {
	Command("@echo \"\e[32m==> compiling source \e[33m[$<]\e[0m\""),
	Command("@mkdir -p $(@D)"),
	Command("@$(CC) -c -o $@ $< $(COPTS) -MMD")
	}, 0));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* cleaning rule */
  makefile.put_element(new Rule("clean", "", {
	Command("rm -f $(OBJECTS) $(DEPENDS)")
	}, Rule::PHONY));

  for (Element* element : elements)
  {
    makefile.put_element(element);
  }
}