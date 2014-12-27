#ifndef UNIXFSHANDLERUNIX_HH_
# define UNIXFSHANDLERUNIX_HH_

# include <sys/types.h>
# include <dirent.h>
# include <unistd.h>

# include "AFSHandler.hpp"

namespace FileSystem
{
namespace Unix
{
class FSHandler : public AFSHandler
{
public:
  FSHandler() = default;
  virtual ~FSHandler() = default;
  std::string currentPath() const override;
  std::list<std::string> listDir(const std::string& dir) const override;
  std::string getPathSeparator() const override {return "/";};
};

};
};

#endif
