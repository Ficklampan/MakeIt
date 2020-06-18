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

static void init_filepath(std::string &filepath)
{
  uint32_t lastSplit = 0;
  for (uint32_t i = 0; i < filepath.size(); i++)
  {
    char c = filepath.at(i);
    if (c == '/')
      lastSplit = i;
  }
  std::string directory = filepath.substr(0, lastSplit + 1);
  std::string fileName, fileExt;
  file_utils::get_file_name(filepath, fileName, fileExt);
  if (fileExt.empty())
    fileExt+='*';
  filepath.clear();
  std::cout << "  checking directory `" << directory << "`\n";
  std::vector<std::string> files = file_utils::find_files(directory, fileName, fileExt);
  for (const std::string &file : files)
    filepath.append(file + " ");
}

static void process_flags(std::vector<std::string> &flags, uint32_t off, uint32_t len)
{
  for (uint32_t i = off; i < len; i++)
  {
    std::string &flag = flags.at(i);
    std::vector<std::string> flag_args = string_utils::split(flag, ';');
    if (flag_args.size() == 0)
      continue;
    if (flag_args.at(0)=="mv") // move
    {
      if (flag_args.size() < 3)
      {
        std::cout << "  [ERR] missing arguments, at line[" << (i + 1) << "]\n";
        continue;
      }
      system(("mv " + flag_args.at(1) + " " + flag_args.at(2)).c_str());
    }else if (flag_args.at(0)=="rm") // remove
    {
      if (flag_args.size() < 2)
      {
        std::cout << "  [ERR] missing arguments, at line[" << (i + 1) << "]\n";
        continue;
      }
      system(("rm " + flag_args.at(1)).c_str());
    }else if (flag_args.at(0)=="upkg") // unpack
    {

    }
  }
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
    }else if (line=="makefile:")
    {
      std::cout << "==> [INFO] constructing makefile...\n";
      get_elements(lines, i, level, elements);
      if (elements.size() < 1)
      {
        std::cout << "  [ERR] no arguments. at line[" << (i + 1) << "]\n";
        return 0;
      }else if (elements.size() < 4)
      {
        std::cout << "  [ERR] too few arguments. at line[" << (i + 1) << "]\n";
        return 0;
      }
      project->exec = new makeit::execute;
      project->exec->version = elements.at(0);
      project->exec->sources = elements.at(1);
      project->exec->headers = elements.at(2);
      project->exec->libs = elements.at(3);
      init_rvalue(project, project->exec->sources, directory);
      init_rvalue(project, project->exec->headers, directory);
      init_rvalue(project, project->exec->libs, directory);
      makefile(project, directory);
      std::cout << "==> [INFO] Makefile successfully constructed!\n";
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
        std::cout << "  no dependencies found.\n";
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
          std::cout << "  [ERR] name not specified. at line[" << (i + 1) << "]\n";
          continue;
        }
        std::string link = args.at(0);
        std::string name = args.at(1);

        std::string depFilepath = depFolder + name;
        if (boost::filesystem::is_directory(depFilepath))
        {
          std::cout << "    `" << depFilepath << "` already exists, ignored.\n";
          continue;
        }
        if (string_utils::starts_with(link, "git@"))
        {
          std::cout << "    cloning `" << link << "` to `" << depFilepath << "`\n";
          system(("git clone " + link + " " + depFilepath).c_str());
        }else
        {
          std::cout << "    downloading `" << link << "` to `" << depFilepath << "`\n";

        }
        if (args.size() > 2)
          process_flags(args, 2, args.size());
      }
    }else if (line=="verify:")
    {
      get_elements(lines, i, level, elements);
      if (elements.size() < 1)
      {
        std::cout << "  [ERR] no target specified. at line[" << (i + 1) << "]\n";
        return 0;
      }
      bool abort = false;
      for (std::string &var_name : elements)
      {
        std::string &var_value = project->vars[var_name];
        std::vector<std::string> var_elements = string_utils::split(var_value, ' ');
        unsigned int found = 0;
        std::vector<std::string> missing;
        for (const std::string &str : var_elements)
        {
          if (!boost::filesystem::exists(str))
            missing.push_back(str);
          else
            found++;
        }
        std::cout << found << "/" << var_elements.size() << " files found.\n";
        for (const std::string &str : missing)
          std::cout << "  [ERR] `" << str << "` not found\n";
        if (!missing.empty())
          abort = true;
      }
      if (abort)
        return 0;
    }else if (line=="define:")
    {
      get_elements(lines, i, level, elements);
      for (const std::string &str : elements)
      {
        std::vector<std::string> args = string_utils::split(str, ' ');
        if (args.size() == 0)
          continue;
        std::string def_name = args.at(0);
        project->definitions.append("-D" + def_name + " ");
      }
    }else if (line=="do:")
    {
      get_elements(lines, i, level, elements);
      for (std::string &element : elements)
        init_rvalue(project, element, directory);
      process_flags(elements, 0, elements.size());
    }else if (line=="find:")
    {
      get_elements(lines, i, level, elements);
      if (elements.size() < 2)
      {
        std::cout << "  [ERR] too few arguments. at line[" << (i + 1) << "]\n";
        return 0;
      }
      std::string append_to = elements.at(0);
      for (uint32_t i = 1; i < elements.size(); i++)
      {
        std::string element = elements.at(i);
        init_rvalue(project, element, directory);
        init_filepath(element);
        project->vars[append_to].clear();
        project->vars[append_to].append(element);
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
  source.append("HEADERS = ");
  source.append(project->exec->headers + "\n");
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
  source.append("DEFS = " + project->definitions + "\n");

  source.append("LIBS = ");
  for (std::string &str : lib_array)
    source.append("-l" + str + " ");
  source.append("\n\n");

  /* compile sources */
  source.append("$(OUTD)/%.o: %.cpp $(HEADERS)\n");
  source.append("\t$(CC) -c -o $@ $< $(CFLAGS) $(INCD) $(DEFS)\n\n");

  /* link */
  source.append("$(NAME): $(OBJ)\n");
  source.append("\t$(CC) -o $@ $^ $(LIBD) $(LIBS)\n\n");

  /* cleanup */
  source.append(".PHONY: clean\n\n");
  source.append("clean:\n");
  source.append("\trm -f $(OBJ)\n");

  file_utils::write_file(directory + "Makefile", (unsigned char*) &source.at(0), source.size());
  return 1;
}
