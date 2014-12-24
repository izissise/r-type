#ifndef	DLLOADERUNIX_HPP_
# define	DLLOADERUNIX_HPP_

# include	<dlfcn.h>

# include	"IDLLoader.hpp"

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

		_handle = dlopen(lib.c_str(), RTLD_LAZY);
		if (_handle == NULL)
			std::cout << "DLLoader ERROR : can't load " << name << std::endl;
		else
		{
			fct = reinterpret_cast<T *(*)()> (dlsym(_handle, "Create"));
			if (fct == NULL)
				std::cout << "DLLoader ERROR : can't init obj" << std::endl;
		}
		_obj = fct();
	}

	~DLLoader()
	{
		if (_handle)
			close(_handle);
	}

	const T	&GetInstance()
	{
		return _obj;
	}

protected:
};


/*
template<typename T>
DLLoader<T>::DLLoader(const std::string& lib)
{
  _libname = std::string("./") + lib;
  _handle = NULL;
  _loaded = false;
  _symbolname = "instance";
}

template<typename T>
DLLoader<T>::~DLLoader()
{
  char* err;

  if (_loaded)
    if (dlclose(_handle))
      {
        err = dlerror();
        throw nFault(std::string("Can't close library: ") + err, false);
      }
}

template<typename T>
T DLLoader<T>::getInstance()
{
  void* sym;
  T ret;
  T (*func)();

  sym = loadSym(_symbolname);
  func = reinterpret_cast<T (*)()>(sym);
  ret = NULL;
  if (func != NULL)
    ret = func();
  return (ret);
}

template<typename T>
void DLLoader<T>::loadLib()
{
  char* err;

  _handle = dlopen(_libname.c_str(), RTLD_LAZY);
  if (_handle == NULL)
    {
      err = dlerror();
      throw nFault(std::string("Can't load library: ") + err, false);
    }
  _loaded = true;
}

template<typename T>
void* DLLoader<T>::loadSym(const std::string& symbolname)
{
  void* sym;
  char* err;

  if (!_loaded)
    loadLib();
  dlerror();
  sym = dlsym(_handle, symbolname.c_str());
  if (sym == NULL && (err = dlerror()) == NULL)
    throw nFault(std::string("Can't get symbol: ") + err, false);
  return (sym);
}
*/

#endif