#ifdef _WIN32

# ifndef	DLLOADERWIN_HH_
# define	DLLOADERWIN_HH_

# include	"IDLLoader.hh"
# include	<windows.h>

template<typename T>
class DLLoader : public IDLLoader<T>
{
public:
	DLLoader(const std::string &name);
	~DLLoader();
	const T	&GetInstance();

protected:
private:
	HMODULE	_handle;
	T		_obj;
};

# endif // !DLLOADERWIN_HH_
#endif
