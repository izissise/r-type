#ifdef __unix__

# ifndef	DLLOADERUNIX_HPP_
# define	DLLOADERUNIX_HPP_

# include	"IDLLoader.hpp"
# include	<dlfcn.h>

template<typename T>
class DLLoader //: public IDLLoader<T>
{
private:
	void	*_handle;
	T		*_obj;

public:
	DLLoader(const std::string &name)
	{
		T	*(*fct)();
		std::string	lib;

		this->_handle = dlopen(lib.c_str(), RTLD_LAZY);
		if (this->_handle == NULL)
			std::cout << "DLLoader ERROR : can't load " << name << std::endl;
		else
		{
			fct = reinterpret_cast<T *(*)()> (dlsym(this->_handle, "Create"));
			if (fct == NULL)
				std::cout << "DLLoader ERROR : can't init obj" << std::endl;
		}
		this->_obj = fct();
	}

	~DLLoader()
	{
		if (this->_handle)
			close(this->_handle);
	}

	const T	&GetInstance()
	{
		return this->_obj;
	}

protected:
};

# endif // !DLLOADERUNIX_HPP_
#endif