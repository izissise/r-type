#ifndef DLLOADERFACTORY_HPP_INCLUDED
# define DLLOADERFACTORY_HPP_INCLUDED

# include "ADLLoader.hpp"

# include "RTypeConfig.h"
# ifdef UNIX
#  include "Unix/DLLoaderUnix.hpp"
# endif
# ifdef WIN32
#  include "Win/DLLoaderWin.hpp"
# endif


namespace DynamicLibrary
{

class DLLoaderFactory
{
public:
  DLLoaderFactory() = delete;
  ~DLLoaderFactory() = delete;

  template<typename MODULE>
  static std::unique_ptr<DynamicLibrary::ADLLoader<MODULE>> createLoader(const std::string& libPath, const std::string& symbolName)
  {
#ifdef UNIX
    return std::unique_ptr<Unix::DLLoader<MODULE>>(new Unix::DLLoader<MODULE>(libPath, symbolName));
#endif
#ifdef WIN32
    return std::unique_ptr<Win::DLLoader<MODULE>>(new Win::DLLoader<MODULE>(libPath, symbolName));
#endif
  };
};
};

#endif // DLLOADERFACTORY_HPP_INCLUDED
