#include "GNUMake.hpp"

#include "../System.hpp"

/* TODO */
/*
 * support for other compilers
*/

int MI::GNUMake_config(const std::string &filepath, const std::string &compiler, me::File &build_dir, std::vector<std::string*> &flags, std::vector<std::string*> &sources, std::vector<std::string*> &headers, Project* project)
{
#ifndef GNUMAKE_APPEND_ARRAY
  #define GNUMAKE_APPEND_ARRAY(v, c, s) { \
    for (uint32_t i = 0; i < c; i++) \
    { \
      v; \
      if (i < c - 1) \
	s; \
    } \
    content += '\n'; \
  }
#endif

  if (!build_dir.exists())
    build_dir.mkdir();

  std::vector<me::File> objects;
  for (std::string* src : sources)
    objects.emplace_back(build_dir.getPath(), me::File::extension(*src, "o"));

  std::string content;
  content.reserve(2048);

  content.append("NAME = ").append(project->name) += '\n';

  content += '\n';

  content.append("# compiler\n");
  content.append("CC = ").append(compiler) += '\n';

  /* append flags */
  content.append("FLAGS = ");
  GNUMAKE_APPEND_ARRAY(content.append(*flags.at(i)), flags.size(), content += ' ');

  /* append definitions */
  content.append("# definitions\n");
  content.append("DEFS = ");
  GNUMAKE_APPEND_ARRAY(content.append("-D").append(*project->definitions.at(i)), project->definitions.size(), content += ' ');

  content += '\n';


  /* append libraries */
  content.append("# libraries\n");
  content.append("LIBS = ");
  GNUMAKE_APPEND_ARRAY(content.append("-l").append(*project->libraries.at(i)), project->libraries.size(), content += ' ');

  /* append library paths */
  content.append("# library paths\n");
  content.append("LPATHS = ");
  GNUMAKE_APPEND_ARRAY(content.append("-L").append(*project->library_paths.at(i)), project->library_paths.size(), content += ' ');

  /* append includes */
  content.append("# includes\n");
  content.append("INCS = ");
  GNUMAKE_APPEND_ARRAY(content.append("-i").append(*project->includes.at(i)), project->includes.size(), content += ' ');

  /* append include paths */
  content.append("# include paths\n");
  content.append("IPATHS = ");
  GNUMAKE_APPEND_ARRAY(content.append("-I").append(*project->include_paths.at(i)), project->include_paths.size(), content += ' ');

  content += '\n';


  /* append objects array */
  content.append("OBJECTS = ");
  GNUMAKE_APPEND_ARRAY(content.append(objects.at(i).getPath()), objects.size(), content.append(" \\\n\t"));

  content += '\n';

  /* append headers array */
  content.append("HEADERS = ");
  GNUMAKE_APPEND_ARRAY(content.append(*headers.at(i)), headers.size(), content.append(" \\\n\t"));

  content += '\n';
  content += '\n';

  content.append("# linking\n");

  /* linking stuff */

  content.append("$(NAME): $(OBJECTS)\n");
  content.append("\t@$(CC) -o $@ $^ $(LPATHS) $(LIBS) $(FLAGS)\n");

  content += '\n';

  content.append(".PHONY: clean\n\n");

  content.append("clean:\n\trm -f $(OBJECTS)\n");

  content += '\n';


  /* compiling stuff */

  content.append("# compiling sources\n\n");
  for (uint32_t i = 0; i < sources.size() && i < objects.size(); i++)
  {
    std::string object = objects.at(i).getPath();
    std::string &source = *sources.at(i);

    content.append(object).append(": ").append(source).append(" $(HEADERS)\n");
    content.append("\t@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo \"\e[32mcompileing [$<]\e[0m\"\n\n");

  }

  me::File file(filepath);
  if (!writeFile(file, content.data(), content.size()))
    return 0;
  return 1;
}
