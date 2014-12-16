#ifdef __unix__

# ifndef	DLLOADERUNIX_HH_
# define	DLLOADERUNIX_HH_

# include	"IDLLoader.hh"
# include	<dlfcn.h>

template<typename T>
class DLLoader : public IDLLoader<T>
{
public:
	DLLoader(const std::string &name);
	~DLLoader();
	const T	&GetInstance();

protected:
private:
	void	*_handle;
	T		_obj;
};

# endif // !DLLOADERUNIX_HH_
#endif