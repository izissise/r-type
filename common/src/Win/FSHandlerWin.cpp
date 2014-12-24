#include "Win/DirHandlerWin.hpp"

std::list<std::string> DirHandler::GetFilesFromDir(const std::string &dir) const
{
  std::list<std::string> list;
  WIN32_FIND_DATA ffd;
  HANDLE hfind;

  hfind = FindFirstFile(dir.c_str(), &ffd);
  if (hfind == INVALID_HANDLE_VALUE)
    std::cout << "" << std::endl;
  else
    {
      while (hfind != INVALID_HANDLE_VALUE)
        {
          std::string	name = ffd.cFileName;

          list.push_back(name);
          if (!FindNextFile(hfind, &ffd))
            {
              FindClose(hfind);
              hfind = INVALID_HANDLE_VALUE;
            }
        }
    }
  return list;
}