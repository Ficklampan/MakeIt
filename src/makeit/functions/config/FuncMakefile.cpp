#include "../Functions.hpp"

#include "../FuncUtils.hpp"

#include "../../configs/make/Makefile.hpp"
#include "../../configs/pc/PkgConfig.hpp"

#include "../../Project.hpp"
#include "../../System.hpp"

#ifdef _WIN32
  #define PKG_CONFIG_PATH_SEPARATOR ';'
#else
  #define PKG_CONFIG_PATH_SEPARATOR ':'
#endif

extern makeit::Config config;

static void GENERATE_MAKEFILE(makeit::gnu_make::Makefile &makefile, makeit::Project &project, std::vector<makeit::gnu_make::Element*> &elements);

static void WRITE_MAKEFILE(const me::File &file, makeit::gnu_make::Makefile &makefile);

makeit::Function* makeit::function::make_makefile()
{
  return new Function(
      {
      new Argument(Variable::STRING),
      new ListArg(
	  new StructArg({
	      StructArg::Arg("pos", false, new Argument(Variable::INTEGER)),
	      StructArg::Arg("lines", true, new ListArg(new Argument(Variable::STRING)))
	      })
	  , Argument::OPTIONAL)
      }, exec_makefile);
}

int makeit::function::exec_makefile(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  me::File file = *args.at(0)->as_string();

  std::vector<gnu_make::Element*> elements;
  if (args.size() > 1)
  {
    Variable::v_list &list = *args.at(1)->as_list();
    for (const Variable* var : list)
    {
      Variable::v_struct &st = *var->as_struct();

      Variable* st_value = st["lines"];
      int pos = st.find("pos") == st.end() ? -2 : *st["pos"]->as_integer();

      std::vector<std::string> lines;
      GET_STRINGS(st_value, lines);
      elements.push_back(new gnu_make::Text(lines, false, pos));
    }
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
  std::string out_name = project.name;
  if (project.config.kind == makeit::BuildConfig::EXECUTABLE)
  {
#ifdef _WIN32 
    out_name.append(".exe"); 
#endif
  }
  if (project.config.kind == makeit::BuildConfig::STATIC_LIBRARY)
    out_name.append(".a");
  else if (project.config.kind == makeit::BuildConfig::SHARED_LIBRARY)
    out_name.append(".so");


  /* compiler collection */
  std::string cc;
  /* GNU GCC */
  if (project.config.cc == makeit::BuildConfig::GNU)
  {
    if (project.config.lang == makeit::BuildConfig::C)
      cc = "gcc";
    else if (project.config.lang == makeit::BuildConfig::CPP)
      cc = "g++";

  /* LLVM CLANG */
  }else if (project.config.cc == makeit::BuildConfig::LLVM)
  {
    if (project.config.lang == makeit::BuildConfig::C)
      cc = "clang";
    else if (project.config.lang == makeit::BuildConfig::CPP)
      cc = "clang++";
  }

  
  makefile.put_element(new Variable("NAME", project.name + "-" + project.version));
  makefile.put_element(new Variable("BUILD", project.config.build.get_path()));
  makefile.put_element(new Variable("OUTNAME", out_name));
  makefile.put_element(new Variable("CC", cc));

  /* empty line */ makefile.put_element(new Text({""}, false));


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

  std::vector<std::string> flags;
  for (const std::string &flag : project.config.flags)
    flags.push_back(flag);

  /* append optimization flag */
  if (project.config.optimization == makeit::BuildConfig::NONE)
    flags.push_back("-O0");
  else if (project.config.optimization == makeit::BuildConfig::O1)
    flags.push_back("-O1");
  else if (project.config.optimization == makeit::BuildConfig::O2)
    flags.push_back("-O2");
  else if (project.config.optimization == makeit::BuildConfig::O3)
    flags.push_back("-O3");
  else if (project.config.optimization == makeit::BuildConfig::Os)
    flags.push_back("-Os");
  else if (project.config.optimization == makeit::BuildConfig::Ofast)
    flags.push_back("-Ofast");
  else if (project.config.optimization == makeit::BuildConfig::Og)
    flags.push_back("-Og");
  else if (project.config.optimization == makeit::BuildConfig::Oz)
    flags.push_back("-Oz");
  /* ------------------------ */

  /* check if project is C++ */
  if (project.config.lang == makeit::BuildConfig::CPP)
  {
    /* append cpp version flag */
    if (project.config.cpp_version == makeit::BuildConfig::CPP_98)
      flags.push_back("-std=c++98");
    else if (project.config.cpp_version == makeit::BuildConfig::CPP_11)
      flags.push_back("-std=c++11");
    else if (project.config.cpp_version == makeit::BuildConfig::CPP_14)
      flags.push_back("-std=c++14");
    else if (project.config.cpp_version == makeit::BuildConfig::CPP_17)
      flags.push_back("-std=c++17");
    else if (project.config.cpp_version == makeit::BuildConfig::CPP_20)
      flags.push_back("-std=c++20");
    /* ------------------------ */
  }

  /* flags */
  makefile.put_element(new Variable("CFLAGS", flags));
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

  /* empty line */ makefile.put_element(new Text({""}, false));

  /* pkg config paths */
  std::string pkg_config_path;
  for (const std::string &lib : project.config.library_paths)
    pkg_config_path.append(lib) += PKG_CONFIG_PATH_SEPARATOR;
  for (const std::string &inc : project.config.include_paths)
    pkg_config_path.append(inc) += PKG_CONFIG_PATH_SEPARATOR;
  makefile.put_element(new Variable("PKG_CONFIG_PATH", pkg_config_path));

  /* compiler packages */
  std::vector<std::string> cpackage_configs;
  for (const makeit::CPackageConfig &config : project.config.cpackage_configs)
  {
    std::string flag = "$$(env PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config ";

    /* append all package flags */
    for (uint32_t i = 0; i < config.flags.size(); i++)
      flag.append(config.flags.at(i)) += ' ';

    flag.append(config.name) += ')';
    cpackage_configs.push_back(flag);
  }
  makefile.put_element(new Variable("CPKG", cpackage_configs));

  /* linker packages */
  std::vector<std::string> lpackage_configs;
  for (const makeit::LPackageConfig &config : project.config.lpackage_configs)
  {
    std::string flag = "$$(env PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config ";
    
    if (config.static_)
      flag.append("--static ");

    /* append all package flags */
    for (uint32_t i = 0; i < config.flags.size(); i++)
      flag.append(config.flags.at(i)) += ' ';

    flag.append(config.name) += ')';
    lpackage_configs.push_back(flag);
  }
  makefile.put_element(new Variable("LPKG", lpackage_configs));
  /* empty line */ makefile.put_element(new Text({""}, false));


  /* external configs */
  std::vector<std::string> extern_configs;
  for (const makeit::ExternConfig &config : project.config.extern_configs)
  {
    extern_configs.push_back(config.target);
  }
  makefile.put_element(new Variable("EXTERN", extern_configs));


  /* empty line */ makefile.put_element(new Text({""}, false));


  /* compiler options */
  makefile.put_element(new Variable("COPTS", { "$(CFLAGS)", "$(IPATHS)", "$(INCS)", "$(DEFS)", "$(CPKG)" }));
  /* linker options */
  makefile.put_element(new Variable("LOPTS", { "$(LPATHS)", "$(LIBS)", "$(LPKG)" }));


  /* empty line */ makefile.put_element(new Text({""}, false));

  /* source files */
  makefile.put_element(new Variable("SOURCES", project.config.sources));

  /* empty line */
  makefile.put_element(new Text({""}, false));

  /* object files */
  makefile.put_element(new Variable("OBJECTS", "$(SOURCES:%=$(BUILD)/%.o)"));
  /* dependency files */
  makefile.put_element(new Variable("DEPENDS", "$(OBJECTS:%.o=%.d)"));


  /* empty line */ makefile.put_element(new Text({""}, false));

  /* build rule */
  makefile.put_element(new Rule("$(NAME)", "$(EXTERN) $(OUTNAME)", {
	}, Rule::PHONY));

  /* empty line */ makefile.put_element(new Text({""}, false));

  /* linking rule */
  if (project.config.kind == makeit::BuildConfig::EXECUTABLE)
  {
    makefile.put_element(new Rule("$(OUTNAME)", "$(OBJECTS)", {
	  Command("@$(CC) -o $@ $^ $(LOPTS)")
	  }, 0));

  /* static library rule */
  }else if (project.config.kind== makeit::BuildConfig::STATIC_LIBRARY)
  {
    makefile.put_element(new Rule("$(OUTNAME)", "$(OBJECTS)", {
	Command("@ar rcs $@ $^")
	}, 0));

  /* shared library rule */
  }else if (project.config.kind == makeit::BuildConfig::SHARED_LIBRARY)
  {
    makefile.put_element(new Rule("$(OUTNAME)", "$(OBJECTS)", {
	Command("@$(CC) -shared -o $@ $^ $(LOPTS)")
	}, 0));
  }

  /* empty line */ makefile.put_element(new Text({""}, false));

  /* include */
  makefile.put_element(new Text({"-include $(DEPENDS)"}, false));

  /* empty line */ makefile.put_element(new Text({""}, false));

  /* compiling rule */
  makefile.put_element(new Rule("$(BUILD)/%.o", "%", {
	Command("@echo \"\e[32m==> compiling source \e[33m[$<]\e[0m\""),
	Command("@mkdir -p $(dir $@)"),
	Command("@$(CC) -c -o $@ $< $(COPTS) -MMD")
	}, 0));

  /* empty line */ makefile.put_element(new Text({""}, false));

  for (const makeit::ExternConfig &config : project.config.extern_configs)
  {

    /* comment of the extern config name */
    if (!config.name.empty())
      makefile.put_element(new Text({"[" + config.name + "]"}, true));

    std::vector<Command> commands;
    commands.emplace_back(std::string("@echo \"\e[97mexternal target '$<'\e[0m\""));

    for (const std::string command : config.commands)
      commands.emplace_back(command);

    makefile.put_element(new Rule(config.target, config.prerequisites, commands, config.phony ? Rule::PHONY : 0));

    /* empty line */ makefile.put_element(new Text({""}, false));
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
