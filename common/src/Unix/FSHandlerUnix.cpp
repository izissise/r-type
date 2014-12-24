#include "Unix/FSHandlerUnix.hpp"

#include <cerrno>

namespace FileSystem
{
namespace Unix
{

std::string FSHandler::currentPath() const
{
  char *res;

  res = getcwd(nullptr, 0);
  if (res == nullptr)
    throw FileSystemError(std::string("currentPath") + strerror(errno));
  return res;
}

std::list<std::string> FSHandler::listDir(const std::string& dir) const
{
  std::list<std::string> list;
  DIR					 *dfd;
  struct dirent			 *file;

  dfd = opendir(dir.c_str());
  if (dfd == nullptr)
    throw FileSystemError(dir + strerror(errno));
  while ((file = readdir(dfd)) != nullptr)
    {
      list.push_back(file->d_name);
    }
  closedir(dfd);
  return list;
}

};
};
