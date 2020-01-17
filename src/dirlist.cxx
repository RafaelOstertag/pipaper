#include "dirlist.hh"

#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <stdexcept>
#include <sys/types.h>

class DirectoryHandle {
  public:
    DirectoryHandle(const char* path) : dir{nullptr} {
        dir = opendir(path);
        if (dir == nullptr) {
            throw std::invalid_argument(std::strerror(errno));
        }
    }
    DirectoryHandle(const DirectoryHandle&) = delete;
    DirectoryHandle& operator=(const DirectoryHandle&) = delete;
    DirectoryHandle(DirectoryHandle&&) = delete;
    DirectoryHandle& operator=(DirectoryHandle&&) = delete;

    ~DirectoryHandle() {
        if (dir != nullptr) {
            closedir(dir);
            dir = nullptr;
        }
    }

    DIR* operator*() { return dir; }

  private:
    DIR* dir;
};

DirectoryList::DirectoryList(const std::string& path) : path{path} {}

std::vector<std::string> DirectoryList::list() const {
    DirectoryHandle directoryHandle(path.c_str());

    std::vector<std::string> list;
    struct dirent* dent = nullptr;
    while ((dent = readdir(*directoryHandle)) != nullptr) {
        if ((dent->d_type & DT_REG) != DT_REG) {
            continue;
        }

        list.push_back(path + "/" + dent->d_name);
    }

    return list;
}
