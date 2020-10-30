#ifndef MELIB_FILE_HPP
  #define MELIB_FILE_HPP

#include <sys/stat.h>

#include <string>
#include <vector>
#include <cstdint>

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

    size_t listFiles(std::vector<File*> &files);

    void normalize();

    int read(void* &data, uint32_t& size);
    int write(void* data, uint32_t size);

  };

}

#endif
