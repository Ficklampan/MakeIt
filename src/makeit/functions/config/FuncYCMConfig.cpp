#include "../Functions.hpp"

#include "../FuncUtils.hpp"

#include "../../configs/ycm/YCMConfig.hpp"

#include "../../Project.hpp"
#include "../../System.hpp"

static void GENERATE_YCMCONFIG(makeit::ycm::YCMConfig &ycm, makeit::Project &project);
static void WRITE_YCMCONFIG(const me::File &file, makeit::ycm::YCMConfig &ycm);

makeit::Function* makeit::function::make_ycm_config()
{
  return new Function({
      new Argument(Variable::STRING)
      }, exec_ycm_config);
}
int makeit::function::exec_ycm_config(void* ptr, std::vector<Variable*> &args)
{
  Storage* storage = (Storage*) ptr;

  REQUIRE_VARIABLE("project", Variable::POINTER, storage);

  Project* project = (Project*) storage->variables["project"]->as_pointer();

  me::File file = *args.at(0)->as_string();

  /* generate the YCMConfig */
  ycm::YCMConfig ycm;
  GENERATE_YCMCONFIG(ycm, *project);

  /* write the YCMConfig */
  WRITE_YCMCONFIG(file, ycm);

  return 1;
}

static void GENERATE_YCMCONFIG(makeit::ycm::YCMConfig &ycm, makeit::Project &project)
{
  using namespace makeit::ycm;

  ycm.put_flag(new Flag("", project.config.flags));
  ycm.put_flag(new Flag("-I", project.config.include_paths));
  ycm.put_flag(new Flag("--include=", project.config.includes));
  ycm.put_flag(new Flag("-D", project.config.definitions));
}

static void WRITE_YCMCONFIG(const me::File &file, makeit::ycm::YCMConfig &ycm)
{
  std::string ycm_config_data;
  ycm_config_data.reserve(1024);
  ycm.write(ycm_config_data);
  makeit::writeFile(file, ycm_config_data.data(), ycm_config_data.size());
}
