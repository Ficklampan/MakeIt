#include "../Functions.hpp"

#include "../FuncUtils.hpp"

#include "../../configs/pc/PkgConfig.hpp"

#include "../../Project.hpp"
#include "../../System.hpp"

static void GENERATE_PKGCONFIG(makeit::pc::PkgConfig &pkg, makeit::Project &project);
static void WRITE_PKGCONFIG(const me::File &file, makeit::pc::PkgConfig &pkg);

makeit::Function* makeit::function::make_pkg_config()
{
  return new Function({
      new Argument(Variable::STRING)
      }, exec_pkg_config);
}
int makeit::function::exec_pkg_config(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  me::File file = *args.at(0)->as_string();

  /* generate the PkgConfig */
  pc::PkgConfig pkg;
  GENERATE_PKGCONFIG(pkg, *project);

  /* write the PkgConfig */
  WRITE_PKGCONFIG(file, pkg);

  return 1;
}

static void GENERATE_PKGCONFIG(makeit::pc::PkgConfig &pkg, makeit::Project &project)
{
  using namespace makeit::pc;



  std::vector<std::string> flags;
  std::vector<std::string> libs, incs;

  for (const std::string &flag : project.config.flags)
    flags.push_back(flag);

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

  /* append libraries */
  for (const std::string &lib : project.config.libraries)
    libs.push_back("-l" + lib);
  for (const std::string &lib_path : project.config.library_paths)
    libs.push_back("-L" + lib_path);

  /*
  for (const std::string &inc : project.config.includes)
    libs.push_back("--include=" + inc);
  for (const std::string &inc_path : project.config.include_paths)
    libs.push_back("-I" + inc_path);
  */



  pkg.put_flag(new Flag("Name", project.name));
  pkg.put_flag(new Flag("Description", project.description));
  pkg.put_flag(new Flag("Version", project.version));
  pkg.put_flag(new Flag("Libs", libs));
  //pkg.put_flag(new Flag("Incs", incs));
  pkg.put_flag(new Flag("Cflags", flags));
}

static void WRITE_PKGCONFIG(const me::File &file, makeit::pc::PkgConfig &pkg)
{
  std::string pkg_config_data;
  pkg_config_data.reserve(1024);
  pkg.write(pkg_config_data);
  makeit::writeFile(file, pkg_config_data.data(), pkg_config_data.size());
}
