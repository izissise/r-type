#ifdef _WIN32
# include	"DirHandlerWin.hh"
# include	<windows.h>
# include	<iostream>

DirHandler	*DirHandler::instance = NULL;

DirHandler				*DirHandler::GetInstance()
{
	if (instance == NULL)
		instance = new DirHandler();
	return (instance);
}

std::list<std::string>	DirHandler::GetFilesFromDir(const std::string &dir) const
{
	std::list<std::string>	list;
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
	return (list);
}

std::list<std::string>	DirHandler::GetLibsFromDir(const std::string &dir) const
{
	std::list<std::string>	list;
	WIN32_FIND_DATA ffd;
	HANDLE hfind;

	hfind = FindFirstFile(dir.c_str(), &ffd);
	if (hfind == INVALID_HANDLE_VALUE)
		std::cout << "error" << std::endl;
	else
	{
		while (hfind != INVALID_HANDLE_VALUE)
		{
			std::string	name = ffd.cFileName;

			std::cout << name << std::endl;
			if (name.find(".so") != std::string::npos)
				list.push_back(name);
			if (!FindNextFile(hfind, &ffd))
			{
				FindClose(hfind);
				hfind = INVALID_HANDLE_VALUE;
			}
		}
	}
	return (list);
}

#endif // _WIN32
