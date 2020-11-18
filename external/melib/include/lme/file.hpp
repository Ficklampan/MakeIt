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
#include <functional>

namespace me {

  class File {

  private:

    std::string path;
    uint8_t prefixLength;

    std::string directory;

  public:

    File(const std::string &path);
    File(const std::string &parent, const std::string &child);



    const std::string absolutePath() const;
    const std::string directoryPath() const;
    const std::string canonicalPath() const;

    int mkdir() const;
    int mkdirs() const;
    int mkfile() const;

    bool exists() const;
    bool isFile() const;
    bool isAbsolutePath() const;
    bool isDirectory() const;

    const std::string& getPath() const;
    int getType() const;

    size_t listFiles(bool sub_dirs, std::function<int(File&)> &callback) const;


    static int read(const File &file, void* &data, uint32_t& size);
    static int write(const File &file, void* data, uint32_t size);

    static void normalize(std::string &path);

    static std::string extension(const std::string &filename, const std::string &ext);

  };

}

#endif
