#include "makeit_parser.h"
#include "file_utils.h"
#include "string_utils.h"

#include <iostream>
#include <stdlib.h>
#include <boost/filesystem.hpp>

static const std::string BUILD_PATH = "MakeItFiles";

static void get_elements(std::vector<std::string> &lines, unsigned int &pos, unsigned int level, std::vector<std::string> &elements)
{
  bool start = false;
  while(pos < lines.size())
  {
    std::string &line = lines.at(pos);
    if (!start && line.at(line.size() - 1) == ':')
    {
      start = true;
      pos++;
      continue;
    }
    if (start && (line.empty() || !(line.at(0) == ' ' && line.at(1) == ' ')))
      break;
    unsigned int in = string_utils::index(line, ' ');
    elements.push_back(line.substr(in));
    pos++;
  }
}

static void init_rvalue(makeit::project* project, std::string &rvalue, std::string &directory)
{
  for (const auto &[key, value] : project->vars)
    string_utils::replace(rvalue, "${" + key + "}", value);
  string_utils::replace(rvalue, "${CURRENT_DIR}", (directory.at(directory.size() - 1) == '/' ? directory.substr(0, directory.size() - 1) : directory));
}

int makeit::parse(makeit::project* project, std::vector<std::string> lines, std::string &directory, unsigned int level, unsigned int offset, unsigned int length)
{
  std::vector<std::string> elements;
  for (unsigned int i = offset; i < length; i++)
  {
    std::string &line = lines.at(i);
    if (!elements.empty())
      elements.clear();
    if (line=="project:")
    {
      get_elements(lines, i, level, elements);
      project = new makeit::project(elements.at(0));
    }else if (line=="append:" || line=="set:")
    {
      get_elements(lines, i, level, elements);
      std::string append_to = elements.at(0);
      if (line=="set:")
        project->vars.clear();
      for (unsigned int j = 1; j < elements.size(); j++)
      {
        project->vars[append_to].append(elements.at(j) + " ");
        init_rvalue(project, project->vars[append_to], directory);
      }
    }else if (line=="var:")
    {
      get_elements(lines, i, level, elements);
      for (const std::string &element : elements)
        project->vars[element] = "";
    }else if (line=="directory:")
    {
      get_elements(lines, i, level, elements);
      for (std::string &element : elements)
      {
        init_rvalue(project, element, directory);
        parse(project, element);
      }
    }else if (line=="exec:")
    {
      std::cout << "==> [INFO] creating executable...\n";
      get_elements(lines, i, level, elements);
      project->exec = new makeit::execute;
      project->exec->version = elements.at(0);
      project->exec->sources = elements.at(1);
      project->exec->libs = elements.at(2);
      init_rvalue(project, project->exec->sources, directory);
      init_rvalue(project, project->exec->libs, directory);
      makefile(project, directory);
      std::cout << "==> [INFO] Makefile created\n";
    }else if (line=="cout:")
    {
      get_elements(lines, i, level, elements);
      for (std::string &element : elements)
      {
        init_rvalue(project, element, directory);
        std::cout << "==> [INFO] " << element << "\n";
      }
    }else if (line=="include:" || line=="library:")
    {
      get_elements(lines, i, level, elements);
      for (std::string &element : elements)
      {
        init_rvalue(project, element, directory);
        if (line=="include:")
          project->includeDirs.append("-I" + element + " ");
        else
          project->libraryDirs.append("-L" + element + " ");
      }
    }else if (line=="dependencies:")
    {
      std::cout << "==> [INFO] checking dependencies...\n";
      get_elements(lines, i, level, elements);
      init_rvalue(project, elements.at(0), directory);
      std::string depFolder = elements.at(0);
      depFolder = string_utils::fix_path(depFolder, true);
      if (elements.size() > 1)
        std::cout << "==> [INFO] downloading dependencies...\n";
      else
      {
        std::cout << "==> [WARN] no dependencies found\n";
        return 0;
      }
      for (unsigned int j = 1; j < elements.size(); j++)
      {
        init_rvalue(project, elements.at(j), directory);
        if (elements.at(j).empty())
          continue;
        std::vector<std::string> args = string_utils::split(elements.at(j), ' ');
        if (args.size() < 2)
        {
          std::cout << "    [ERROR] name not specified, skipping...\n";
          continue;
        }
        std::string link = args.at(0);
        std::string name = args.at(1);
        std::string depFilepath = depFolder + name;
        if (boost::filesystem::is_directory(depFilepath))
        {
          std::cout << "    `" << depFilepath << "` already exists, skipping...\n";
          continue;
        }
        if (string_utils::starts_with(link, "git@"))
        {
          std::cout << "    cloning `" << link << "` to `" << depFilepath << "`\n";
          system(("git clone " + link + " " + depFilepath).c_str());
        }
      }
    }
  }
  return 1;
}

int makeit::parse(makeit::project* project, std::vector<std::string> lines, std::string &directory)
{
  return parse(project, lines, directory, 0, 0, lines.size());
}

int makeit::parse(makeit::project* project, std::string &directory)
{
  if (directory.at(directory.size() - 1) != '/')
    directory+='/';
  std::string filepath = directory + "MakeIt.txt";
  std::string source = std::string((char*) file_utils::read_file(filepath));
  std::vector<std::string> lines = string_utils::split(source, '\n');
  return parse(project, lines, directory);
}

int makeit::makefile(makeit::project* project, std::string &directory)
{
  if (directory.at(directory.size() - 1) != '/')
    directory+='/';

  std::vector<std::string> lib_array = string_utils::split(project->exec->libs, ' ');
  std::vector<std::string> src_array = string_utils::split(project->exec->sources, ' ');

  std::string source;
  source.append("NAME = ");
  source.append(project->name + "\n");
  source.append("SRC = ");
  source.append(project->exec->sources + "\n");
  source.append("OBJ = ");

  for (std::string &str : src_array)
  {
    std::string path = directory + BUILD_PATH + "/" + string_utils::file_extension(str, "o").substr(directory.size());
    source.append(path + " ");
    boost::filesystem::create_directories(string_utils::directory(path));
  }
  source.append("\n\n");

  source.append("CC = g++\n");
  source.append("CFLAGS = -Wall " + project->exec->version + "\n");

  source.append("OUTD = " + BUILD_PATH + "\n");
  source.append("LIBD = " + project->libraryDirs + "\n");
  source.append("INCD = " + project->includeDirs + "\n");

  source.append("LIBS = ");
  for (std::string &str : lib_array)
    source.append("-l" + str + " ");
  source.append("\n\n");

  /* compile sources */
  source.append("$(OUTD)/%.o: %.cpp $(SRC)\n");
  source.append("\t$(CC) -c -o $@ $< $(INCD) $(CFLAGS)\n\n");

  /* link */
  source.append("$(NAME): $(OBJ)\n");
  source.append("\t$(CC) -o $@ $^ $(LIBD) $(LIBS) $(CFLAGS)\n\n");

  /* cleanup */
  source.append(".PHONY: clean\n\n");
  source.append("clean:\n");
  source.append("\trm -f $(OUTD)/*.o\n");

  file_utils::write_file(directory + "Makefile", (unsigned char*) &source.at(0), source.size());
  return 1;
}
