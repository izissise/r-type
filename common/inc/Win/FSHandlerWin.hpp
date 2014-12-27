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
class FSHandler : public AFSHandler
{
public:
  FSHandler() = default;
  virtual ~FSHandler() = default;
  std::string currentPath() const override;
  std::list<std::string> listDir(const std::string& dir) const override;
  std::string getPathSeparator() const override {return "\\";};
};
};
};

#endif
