#ifndef DLMANAGER_HH_
# define	DLMANAGER_HH_

# include	"DLLoaderUnix.hh"
# include	"DLLoaderWin.hh"
# include	<map>

template<typename T>
class DLManager
{
public:
	DLManager(const std::string &rep);
	~DLManager();
	const T	&GetInstance(const std::string &name);

protected:
private:
	bool LoadFromRep();
	std::map<std::string, IDLLoader<T>> modules;
};

#endif // !DLMANAGER_HH_
