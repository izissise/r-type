#include	<iostream>

#include	<windows.h>

#include	"DirHandlerWin.hpp"

DirHandler	*DirHandler::instance = NULL;

DirHandler *DirHandler::GetInstance()
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
	std::string				tmp;
	WIN32_FIND_DATA ffd;
	HANDLE hfind;

	std::cout << "DIR = " << dir << std::endl;
	tmp = dir + "\\*";
	hfind = FindFirstFile(tmp.c_str(), &ffd);
	if (hfind == INVALID_HANDLE_VALUE)
		std::cout << "error" << std::endl;
	else
	{
		while (hfind != INVALID_HANDLE_VALUE)
		{
			std::string	name = ffd.cFileName;

			std::cout << "FILE = " << name << std::endl;
			if (name.find(".dll") != std::string::npos)
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
