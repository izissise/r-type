#ifndef FSHANDLERFACT_HPP_INCLUDED
# define FSHANDLERFACT_HPP_INCLUDED

# include "AFSHandler.hpp"

# include "RTypeConfig.h"
# ifdef UNIX
#  include "Unix/FSHandlerUnix.hpp"
# endif
# ifdef WIN32
#  include "Win/FSHandlerWin.hpp"
# endif


namespace FileSystem
{

class FSFactory
{
public:
  FSFactory() = delete;
  ~FSFactory() = delete;

  static FileSystem::AFSHandler& retrieveFSHandler()
  {
#ifdef UNIX
    return AFSHandler::instance<Unix::FSHandler>();
#endif
#ifdef WIN32
    return AFSHandler::instance<Win::FSHandler>();
#endif
  };
};
};


#endif // FSHANDLER_HPP_INCLUDED
