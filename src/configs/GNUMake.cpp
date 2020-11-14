#include "GNUMake.hpp"

#include <lme/file.hpp>

int MI::GNUMake_config(const std::string &filepath, std::vector<std::string*> &sources, std::vector<std::string*> &headers, Project* project)
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

  if (!project->build_dir->exists())
    project->build_dir->mkdir();

  std::vector<me::File> objects;
  for (std::string* src : sources)
    objects.emplace_back(project->build_dir->getPath(), me::File::extension(*src, "o"));

  std::string content;

  content.append("NAME = ").append(project->name) += '\n';
  content.append("CC = ").append(project->compiler) += '\n';

  /* append flags */
  content.append("FLAGS = ");
  GNUMAKE_APPEND_ARRAY(content.append(project->flag.at(i)), project->flag.size(), content += ' ');

  /* append definitions */
  content.append("DEFS = ");
  GNUMAKE_APPEND_ARRAY(content.append(project->definitions.at(i)), project->definitions.size(), content += ' ');

  content += '\n';


  /* append libraries */
  content.append("LIBS = ");
  GNUMAKE_APPEND_ARRAY(content.append(project->library.at(i)), project->library.size(), content += ' ');

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

  me::File file(filepath);
  if (!file.write(content.data(), content.size()))
    return 0;
  return 1;
}
