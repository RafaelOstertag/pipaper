#ifndef __DIRLIST_HH
#define __DIRLIST_HH

#include <string>
#include <vector>

class DirectoryList {
  public:
    DirectoryList(const std::string& path);
    std::vector<std::string> list() const;

  private:
    std::string path;
};

#endif
