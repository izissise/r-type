#ifndef DIRHANDLERWIN_HH_
# define DIRHANDLERWIN_HH_

# include <list>
# include <string>

# include <windows.h>

# include "AFSHandler.hpp"

namespace FileSystem
{
namespace Win
{
class DirHandler : public AFSHandler
{
public:
  DirHandler() = default;
  ~DirHandler() = default;
  std::string currentPath() const override;
  std::list<std::string> listDir(const std::string& dir) const override;
};
};
};

#endif
