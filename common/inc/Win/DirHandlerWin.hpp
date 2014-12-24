#ifndef	DIRHANDLERWIN_HH_
# define DIRHANDLERWIN_HH_

# include <list>
# include <string>

# include <windows.h>

# include "IDirHandler.hpp"

class DirHandler : public IDirHandler
{
	static DirHandler	*instance;
public:
	DirHandler() {}
	~DirHandler() {}
	static DirHandler		*GetInstance();
	std::list<std::string>	GetFilesFromDir(const std::string &dir) const;
	std::list<std::string>	GetLibsFromDir(const std::string &dir) const;
private:
};

#endif