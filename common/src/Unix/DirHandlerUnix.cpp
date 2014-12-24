#ifdef __unix__
# include	"DirHandlerUnix.hh"
# include	<stdio.h>
# include	<sys/types.h>
# include	<dirent.h>

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
	DIR						*dfd;
	struct dirent			*file;

	dfd = opendir(dir);
	if (dfd == NULL)
	{
		std::cerr << "error can't open dir : " << dir << std::endl;
	}
	else
	{
		while ((file = readdir(dfd)) != NULL)
		{
			std::string	name = file->d_name;

			std::cout << "FILE = " << name << std::endl;
			list.push_back(name);
		}
		closedir(dfd);
	}
	return (list);
}

std::list<std::string>	DirHandler::GetLibsFromDir(const std::string &dir) const
{
	std::list<std::string>	list;
	DIR						*dfd;
	struct dirent			*file;

	dfd = opendir(dir);
	if (dfd == NULL)
	{
		std::cerr << "error can't open dir : " << dir << std::endl;
	}
	else
	{
		while ((file = readdir(dfd)) != NULL)
		{
			std::string	name = file->d_name;

			std::cout << "FILE = " << name << std::endl;
			if (name.find(".lib") != std::string::npos)
				list.push_back(name);
		}
		closedir(dfd);
	}
	return (list);
}

#endif // __unix__
