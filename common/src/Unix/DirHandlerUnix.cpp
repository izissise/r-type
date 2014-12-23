#include "Unix/DirHandlerUnix.hpp"

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

	return (list);
}

std::list<std::string>	DirHandler::GetLibsFromDir(const std::string &dir) const
{
	std::list<std::string>	list;

	return (list);
}
