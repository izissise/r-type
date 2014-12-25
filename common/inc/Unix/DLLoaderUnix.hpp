#ifndef DLLOADERUNIX_HPP_
# define DLLOADERUNIX_HPP_

# include <exception>
# include <stdexcept>
# include <string>
# include <iostream>

# include <dlfcn.h>

#include "ADLLoader.hpp"

namespace DynamicLibrary {
namespace Unix {

template<typename MODULE>
class DLLoader : public ADLLoader<MODULE>
{
public:
  DLLoader(const std::string& libPath, const std::string& symbolName)
    : ADLLoader<MODULE>(libPath, symbolName), _loaded(false), _handle(nullptr)
  {};

  virtual ~DLLoader()
  {
    char* err;

    if (_loaded)
      if (dlclose(_handle))
        {
          err = dlerror();
          std::cerr << "Can't close library: " << err << std::endl;
        }
  };

  //template<typename ... Args>
  std::unique_ptr<MODULE> callSymbol(/*Args&& ... args*/) override
  {
    void* sym;
    MODULE* ret;
    MODULE* (*func)(/*args...*/);

    sym = loadSym(this->_symbolName);
    func = reinterpret_cast<decltype(func)>(sym);
    ret = nullptr;
    if (func != nullptr)
      ret = func();
    return std::unique_ptr<MODULE>(ret);
  };

private:
  void* loadSym(const std::string& symbolname)
  {
    void* sym;
    char* err;

    if (!_loaded)
      loadLib();
    dlerror();
    sym = dlsym(_handle, symbolname.c_str());
    if (sym == nullptr && (err = dlerror()) == nullptr)
      throw std::runtime_error(std::string("Can't get symbol: ") + err);
    return sym;
  };

  void loadLib()
  {
    char* err;

    _handle = dlopen(this->_libName.c_str(), RTLD_LAZY);
    if (_handle == nullptr)
      {
        err = dlerror();
        throw std::runtime_error(std::string("Can't load library: ") + err);
      }
    _loaded = true;
  };

private:
  bool  _loaded;
  void* _handle;
};

};
};

#endif
