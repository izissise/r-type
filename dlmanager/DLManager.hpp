#ifndef DLMANAGER_HPP_
# define	DLMANAGER_HPP_

# include	"DLLoaderUnix.hpp"
# include	"DLLoaderWin.hpp"
# include	"DirHandlerUnix.hh"
# include	"DirHandlerWin.hh"
# include	<map>

template<typename T>
class DLManager
{
private:
	std::map<std::string, DLLoader<T>> modules;
	DirHandler	*dirhandler;

	bool LoadFromRep(const std::string &rep)
	{
		std::list<std::string>	list;

		list = this->dirhandler->GetLibsFromDir(rep);
		if (list.empty())
		{
			std::cout << "List empty" << std::endl;
			return false;
		}
		else
			std::cout << "list not empty" << std::endl;
		for (std::list<std::string>::iterator it = list.begin(); it != list.end(); ++it)
		{
			std::string name = (*it);
			DLLoader<T>	load = DLLoader<T>(name);
			this->modules.insert(std::pair<std::string, DLLoader<T>>(name, load));
		}
		return true;
	}

public:
	DLManager(const std::string &rep)
	{
		this->dirhandler = dirhandler->GetInstance();
		this->LoadFromRep(rep);
	}

	~DLManager()
	{
		if (!this->modules.empty())
			this->modules.clear();
	}

	const T	&GetInstance(const std::string &name)
	{
		return this->modules[name].GetInstance();
	}

protected:
};

#endif // !DLMANAGER_HPP_
