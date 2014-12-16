#ifdef _WIN32

#include	"DLLoaderWin.hh"

template<typename T>
DLLoader<T>::DLLoader(const std::string &name)
{
	T	(*fct)();
	std::string	lib;

	this->_handle = LoadLibrary(lib.c_str());
	if (this->_handle == NULL)
		std::cout << "DLLoader ERROR : can't load " << name << std::endl;
	else
	{
		fct = reinterpret_cast<T(*)()> (GetProcAddress(this->_handle, ""));
		if (fct == NULL)
			std::cout << "DLLoader ERROR : can't init obj" << std::endl;
	}
	this->_obj = fct();
}

template<typename T>
DLLoader<T>::~DLLoader()
{
	if (this->_handle)
		FreeLibrary(this->_handle);
}

template<typename T>
const T	&DLLoader<T>::GetInstance()
{
	return this->_obj;
}

#endif
