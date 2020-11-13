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


  /* append sources array */
  content.append("SOURCES = ");
  GNUMAKE_APPEND_ARRAY(content.append(*sources.at(i)), sources.size(), content.append(" \\\n\t"));

  /* append headers array */
  content.append("HEADERS = ");
  GNUMAKE_APPEND_ARRAY(content.append(*headers.at(i)), headers.size(), content.append(" \\\n\t"));

  me::File file(filepath);
  if (!file.write(content.data(), content.size()))
    return 0;
  return 1;
}
