#ifdef __unix__
# ifndef	DIRHANDLERUNIX_HH_
# define	DIRHANDLERUNIX_HH_

# include	"IDirHandler.hh"

static class DirHandler : public IDirHandler
{
	IDirHandler	*instance = NULL;
public:
	DirHandler() {}
	~DirHandler() {}
	IDirHandler						*GetInstance();
	const std::list<std::string>	&GetFilesFromDir(const std::string &dir) const;
	const std::list<std::string>	&GetLibsFromDir(const std::string &dir) const;

private:
};

# endif
#endif // __unix__
