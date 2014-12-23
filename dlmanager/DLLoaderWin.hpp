#ifdef _WIN32

# ifndef	DLLOADERWIN_HPP_
# define	DLLOADERWIN_HPP_

# include	"IDLLoader.hh"
# include	<windows.h>
# include	<iostream>

template<typename T>
class DLLoader //: public IDLLoader<T>
{
private:
	HINSTANCE	_handle;
	T		*_obj;

public:
	DLLoader(const std::string &name)
	{
		T *(*fct)() = NULL;
		std::string	lib = name;

		this->_handle = LoadLibrary(lib.c_str());
		if (this->_handle == NULL)
			std::cout << "DLLoader ERROR : can't load " << name << std::endl;
		else
		{
			fct = (T *(*)())(GetProcAddress((HMODULE)this->_handle, "Create"));
			if (fct == NULL)
				std::cout << "DLLoader ERROR[" << GetLastError() << "] : can't init obj" << std::endl;

		}
		this->_obj = fct();
	}

	~DLLoader()
	{
		if (this->_handle)
			FreeLibrary(this->_handle);
	}

	const T	*GetInstance()
	{
		return (this->_obj);
	}

protected:
};

# endif // !DLLOADERWIN_HPP_
#endif
