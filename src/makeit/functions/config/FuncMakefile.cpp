#include "../Functions.hpp"

#include "../FuncUtils.hpp"

#include "../../configs/make/Makefile.hpp"
#include "../../configs/pc/PkgConfig.hpp"

#include "../../Project.hpp"
#include "../../System.hpp"

extern makeit::Config config;

static void GENERATE_MAKEFILE(makeit::gnu_make::Makefile &makefile, makeit::Project &project, std::vector<makeit::gnu_make::Element*> &elements);

static void WRITE_MAKEFILE(const me::File &file, makeit::gnu_make::Makefile &makefile);

makeit::Function* makeit::function::make_makefile()
{
  return new Function(
      {
      new Argument(Variable::STRING),
      new StructArg({
	  StructArg::Arg("pos", false, new Argument(Variable::INTEGER)),
	  StructArg::Arg("lines", true, new ListArg(new Argument(Variable::STRING)))
	  }, Argument::ENDLESS | Argument::OPTIONAL)
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

  /* generate the Makefile */
  gnu_make::Makefile makefile;
  GENERATE_MAKEFILE(makefile, *project, elements);

  /* wrtie the Makefile */
  WRITE_MAKEFILE(file, makefile);

  /* execute the Makefile */
  if (config.execute_output)
    makeit::system(std::string("make -f " + file.get_path()).c_str());

  return 1;
}

static void GENERATE_MAKEFILE(makeit::gnu_make::Makefile &makefile, makeit::Project &project, std::vector<makeit::gnu_make::Element*> &elements)
{
  using namespace makeit::gnu_make;

  /* output file */
  std::string outname = project.config.out_name.empty() ? project.name : project.config.out_name;
  if (project.config.out_type == makeit::BuildConfig::STATIC_LIBRARY)
    outname.append(".a");
  else if (project.config.out_type == makeit::BuildConfig::SHARED_LIBRARY)
    outname.append(".so");

  makefile.put_element(new Variable("NAME", project.name + "-" + project.version));
  makefile.put_element(new Variable("BUILD", project.config.build.get_path()));
  makefile.put_element(new Variable("OUTNAME", outname));
  makefile.put_element(new Variable("CC", project.config.cc));

  /* empty line */
  makefile.put_element(new Text({""}, false));


  /* adding a flag prefix to the libraries, includes... */
#define ADD_PREFIX(p, v, o) { \
  for (const std::string &str : v) \
    o.push_back(p + str); \
}

  std::vector<std::string> libs, incs, lib_paths, inc_paths, defs;
  ADD_PREFIX("-l", project.config.libraries, libs);
  ADD_PREFIX("--include=", project.config.includes, incs);
  ADD_PREFIX("-L", project.config.library_paths, lib_paths);
  ADD_PREFIX("-I", project.config.include_paths, inc_paths);
  ADD_PREFIX("-D", project.config.definitions, defs);

#undef ADD_PREFIX
  /* --------------------------------------------------- */


  /* cflags */
  makefile.put_element(new Variable("CFLAGS", project.config.cflags));
  /* lflags */
  makefile.put_element(new Variable("LFLAGS", project.config.lflags));
  /* libraries */
  makefile.put_element(new Variable("LIBS", libs));
  /* includes */
  makefile.put_element(new Variable("INCS", incs));
  /* library paths */
  makefile.put_element(new Variable("LPATHS", lib_paths));
  /* include paths */
  makefile.put_element(new Variable("IPATHS", inc_paths));
  /* definitions */
  makefile.put_element(new Variable("DEFS", defs));


  /* external configs */
  std::vector<std::string> extern_configs;
  for (const makeit::ExternConfig &config : project.config.extern_configs)
  {
    extern_configs.push_back(config.name);
  }
  makefile.put_element(new Variable("EXT", extern_configs));


  /* empty line */
  makefile.put_element(new Text({""}, false));


  /* compiler options */
  makefile.put_element(new Variable("COPTS", { "$(CFLAGS)", "$(INCS)", "$(IPATHS)", "$(DEFS)" }));
  /* linker options */
  makefile.put_element(new Variable("LOPTS", { "$(LFLAGS)", "$(LIBS)", "$(LPATHS)" }));


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

  /* build rule */
  makefile.put_element(new Rule("$(NAME)", "$(EXT) $(OUTNAME)", {
	}, Rule::PHONY));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* linking rule */
  if (project.config.out_type == makeit::BuildConfig::EXECUTABLE)
  {
    makefile.put_element(new Rule("$(OUTNAME)", "$(OBJECTS)", {
	  Command("@$(CC) -o $@ $^ $(LOPTS)")
	  }, 0));

  /* static library rule */
  }else if (project.config.out_type == makeit::BuildConfig::STATIC_LIBRARY)
  {
    makefile.put_element(new Rule("$(OUTNAME)", "$(OBJECTS)", {
	Command("@ar rcs $@ $^")
	}, 0));

  /* shared library rule */
  }else if (project.config.out_type == makeit::BuildConfig::SHARED_LIBRARY)
  {
    makefile.put_element(new Rule("$(OUTNAME)", "$(OBJECTS)", {
	Command("@$(CC) -shared -o $@ $^ $(LOPTS)")
	}, 0));
  }

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* include */
  makefile.put_element(new Text({"-include $(DEPENDS)"}, false));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* compiling rule */
  makefile.put_element(new Rule("$(BUILD)/%.o", "%", {
	Command("@echo \"\e[32m==> compiling source \e[33m[$<]\e[0m\""),
	Command("@mkdir -p $(dir $@)"),
	Command("@$(CC) -c -o $@ $< $(COPTS) -MMD")
	}, 0));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  for (const makeit::ExternConfig &config : project.config.extern_configs)
  {
    std::string command = "";
    switch (config.type)
    {
      case makeit::ExternConfig::MAKEIT: command = "makeit $(notdir $<)"; break;
      case makeit::ExternConfig::CMAKE: command = "cmake $(notdir $<)"; break;
      case makeit::ExternConfig::PREMAKE: command = "premake $(notdir $<)"; break;
      case makeit::ExternConfig::GNUMAKE: command = "make -f $(notdir $<)"; break;
      case makeit::ExternConfig::SHELL: command = "shell $(notdir $<)"; break;
    }

    makefile.put_element(new Rule(config.name, config.target, {
	  Command("@echo \"\e[97mbuilding external target '$<'\e[0m\""),
	  Command("@( cd $(dir $<); " + command + (config.args.empty() ? "" : " " + config.args) + ")")
	  }, Rule::PHONY));

    /* empty line */
    makefile.put_element(new Text({""}, false));
  }

  /* cleaning rule */
  makefile.put_element(new Rule("clean", "", {
	Command("rm -f $(OUTNAME) $(OBJECTS) $(DEPENDS)")
	}, Rule::PHONY));

  for (Element* element : elements)
  {
    makefile.put_element(element);
  }
}

static void WRITE_MAKEFILE(const me::File &file, makeit::gnu_make::Makefile &makefile)
{
  std::string makefile_data;
  makefile_data.reserve(4096);
  makefile.write(makefile_data);
  makeit::writeFile(file, makefile_data.data(), makefile_data.size());
}
