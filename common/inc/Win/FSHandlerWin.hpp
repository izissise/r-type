#ifndef DIRHANDLERWIN_HH_
# define DIRHANDLERWIN_HH_

# include <list>
# include <string>

# include <windows.h>

# include "IDirHandler.hpp"

class DirHandler : public IDirHandler
{
public:
  DirHandler() = default;
  ~DirHandler() = default;
  std::list<std::string> GetFilesFromDir(const std::string &dir) const override;
};

#endif
