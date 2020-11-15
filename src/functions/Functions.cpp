#include "Functions.hpp"

namespace MI { namespace function {

  MI::Function print = {
    1,
    new uint16_t[1]{
      1 | (Constant::VOID << 1)					/* arg[0+]: any type */
    },
    exec_print};
  MI::Function system = {
    1,
    new uint16_t[1]{
      1 | (Constant::STRING << 1) | (Constant::LIST << 4)	/* arg[0+]: type 'string' or 'list' */
    },
    exec_system};
  MI::Function search = {
    4,
    new uint16_t[4]{
      0 | (Constant::STRING << 1) | (Constant::LIST << 4), 	/* arg[0]: type 'string' or 'list' */
      0 | (Constant::STRING << 1) | (Constant::LIST << 4), 	/* arg[1]: type 'string' or 'list' */
      0 | (Constant::BOOLEAN << 1), 				/* arg[2]: type 'boolean' */
      0 | (Constant::LIST << 1) 				/* arg[3]: type 'list' */
    },
    exec_search};
  
  MI::Function project = {
    3,
    new uint16_t[3]{
      0 | (Constant::STRING << 1), 				/* arg[0]: type 'string' */
      0 | (Constant::STRING << 1),				/* arg[1]: type 'string' */
      0 | (Constant::STRING << 1)				/* arg[2]: type 'string' */
    },
    exec_project
  };
  MI::Function library = {
    1,
    new uint16_t[1]{
      1 | (Constant::STRING << 1) | (Constant::LIST << 4) 	/* arg[0+]: type 'string' or 'list' */
    },
    exec_library
  };
  MI::Function library_path = {
    1,
    new uint16_t[1]{
      1 | (Constant::STRING << 1) | (Constant::LIST << 4) 	/* arg[0+]: type 'string' or 'list' */
    },
    exec_library_path
  };
  MI::Function include = {
    1,
    new uint16_t[1]{
      1 | (Constant::STRING << 1) | (Constant::LIST << 4) 	/* arg[0+]: type 'string' or 'list' */
    },
    exec_include
  };
  MI::Function include_path = {
    1,
    new uint16_t[1]{
      1 | (Constant::STRING << 1) | (Constant::LIST << 4) 	/* arg[0+]: type 'string' or 'list' */
    },
    exec_include_path
  };
  MI::Function define = {
    1,
    new uint16_t[1]{
      1 | (Constant::STRING << 1) | (Constant::LIST << 4) 	/* arg[0+]: type 'string' or 'list' */
    },
    exec_define
  };
  MI::Function source = {
    1,
    new uint16_t[1]{
      1 | (Constant::STRING << 1) | (Constant::LIST << 4), 	/* arg[0+1]: type 'string' or 'list' */
    },
    exec_source
  };
  MI::Function makefile = {
    6,
    new uint16_t[6]{
      0 | (Constant::STRING << 1) | (Constant::LIST << 4), 	/* arg[0]: type 'string' or 'list' */
      0 | (Constant::STRING << 1),			 	/* arg[1]: type 'string' */
      0 | (Constant::STRING << 1),			 	/* arg[2]: type 'string' */
      0 | (Constant::STRING << 1) | (Constant::LIST << 4), 	/* arg[3]: type 'string' or 'list' */
      0 | (Constant::STRING << 1) | (Constant::LIST << 4), 	/* arg[4]: type 'string' or 'list' */
      0 | (Constant::STRING << 1) | (Constant::LIST << 4) 	/* arg[5]: type 'string' or 'list' */
    },
    exec_makefile
  };
  MI::Function ycm = {
    0,
    new uint16_t[0]{
    },
    exec_ycm
  };

} }

void MI::function::usage(const std::string &func)
{
  // TODO
}

MI::Function* MI::function::getPrint() { return &print; }
MI::Function* MI::function::getSystem() { return &system; }
MI::Function* MI::function::getSearch() { return &search; }

MI::Function* MI::function::getProject() { return &project; }
MI::Function* MI::function::getLibrary() { return &library; }
MI::Function* MI::function::getLibraryPath() { return &library_path; }
MI::Function* MI::function::getInclude() { return &include; }
MI::Function* MI::function::getIncludePath() { return &include_path; }
MI::Function* MI::function::getDefine() { return &define; }
MI::Function* MI::function::getSource() { return &source; }
MI::Function* MI::function::getMakefile() { return &makefile; }
MI::Function* MI::function::getYCM() { return &ycm; }
