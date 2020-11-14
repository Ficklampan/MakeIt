#ifndef MELIB_FILE_HPP
  #define MELIB_FILE_HPP

#ifndef FILE_SEPARATOR
  #if (defined _WIN32 || defined _WIN64)
    #define FILE_SEPARATOR '\\'
  #else
    #define FILE_SEPARATOR '/'
  #endif
#endif

#include <string>
#include <vector>

namespace me {

  class File {

  private:

    std::string path;
    uint8_t prefixLength;

    std::string directory;

  public:

    File(const std::string &path);
    File(const std::string &parent, const std::string &child);



    std::string absolutePath();
    std::string directoryPath();
    std::string canonicalPath();

    int mkdir();
    int mkdirs();
    int mkfile();

    bool exists();
    bool isFile();
    bool isAbsolutePath();
    bool isDirectory();

    const std::string getPath();
    int getType();

    size_t listFiles(std::vector<File*> &files, bool sub_dirs);

    void normalize();

    int read(void* &data, uint32_t& size);
    int write(void* data, uint32_t size);

    static std::string extension(const std::string &filename, const std::string &ext);

  };

}

#endif
