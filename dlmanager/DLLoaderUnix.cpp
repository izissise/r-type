#ifdef __unix__

#include	"DLLoaderUnix.hh"

template<typename T>
DLLoader<T>::DLLoader(const std::string &name)
{
	T	(*fct)();
	std::string	lib;

	this->_handle = dlopen(lib.c_str(), RTLD_LAZY);
	if (this->_handle == NULL)
		std::cout << "DLLoader ERROR : can't load " << name << std::endl;
	else
	{
		fct = reinterpret_cast<T (*)()> (dlsym(this->_handle, ""));
		if (fct == NULL)
			std::cout << "DLLoader ERROR : can't init obj" << std::endl;
	}
	this->_obj = fct();
}

template<typename T>
DLLoader<T>::~DLLoader()
{
	if (this->_handle)
		close(this->_handle);
}

template<typename T>
const T	&DLLoader<T>::GetInstance()
{
	return this->_obj;
}

#endif
