#ifdef __unix__
# ifndef	DIRHANDLERUNIX_HH_
# define	DIRHANDLERUNIX_HH_

# include	"IDirHandler.hpp"

static class DirHandler : public IDirHandler
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

# endif
#endif // __unix__
