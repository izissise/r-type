#include "Win/FSHandlerWin.hpp"

#include <memory>

namespace FileSystem
{
namespace Win
{

std::string FSHandler::currentPath() const
{
  size_t buffSize = GetCurrentDirectory(0, nullptr);
  if (buffSize == 0)
    throw FileSystemError("currentPath: Unknown error");
  std::unique_ptr<char[]> buff(new char[buffSize]);
  buffSize = GetCurrentDirectory(buffSize, buff.get());
  if (buffSize == 0)
    throw FileSystemError("currentPath: Unknown error");
  return std::string(buff.get()) + "\\";
}


std::list<std::string> FSHandler::listDir(const std::string &dir) const
{
  std::list<std::string> list;
  WIN32_FIND_DATA ffd;
  HANDLE hfind;

  hfind = FindFirstFile(dir.c_str(), &ffd);
  if (hfind == INVALID_HANDLE_VALUE)
    throw FileSystemError(dir + ": can't open directory");
  while (hfind != INVALID_HANDLE_VALUE)
    {
      list.push_back(ffd.cFileName);
      if (!FindNextFile(hfind, &ffd))
        {
          FindClose(hfind);
          hfind = INVALID_HANDLE_VALUE;
        }
    }
  return list;
}

};
};
