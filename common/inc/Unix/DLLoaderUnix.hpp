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
  DLLoader(const std::string& libPath, const std::string& symbolName);
  virtual ~DLLoader();

  template<typename ... Args>
  std::unique_ptr<MODULE> callSymbol(Args&& ... args) const override;

private:
  void* loadSym(const std::string& sym);
  void  loadLib();

private:
  bool  _loaded;
  void* _handle;
};

template<typename MODULE>
DLLoader<MODULE>::DLLoader(const std::string& libPath, const std::string& symbolName)
  : ADLLoader<MODULE>(libPath, symbolName), _loaded(false), _handle(nullptr)
{
}

template<typename MODULE>
DLLoader<MODULE>::~DLLoader()
{
  char* err;

  if (_loaded)
    if (dlclose(_handle))
      {
        err = dlerror();
        std::cerr << "Can't close library: " << err << std::endl;
      }
}

template<typename MODULE>
template<typename ... Args>
std::unique_ptr<MODULE> DLLoader<MODULE>::callSymbol(Args&& ... args) const
{
  void* sym;
  MODULE ret;
  MODULE (*func)(args...);

  sym = loadSym(_symbolname);
  func = reinterpret_cast<MODULE (*)(std::forward<Args>(args)...)>(sym);
  ret = nullptr;
  if (func != nullptr)
    ret = func();
  return std::unique_ptr<MODULE>(ret);
}

template<typename MODULE>
void DLLoader<MODULE>::loadLib()
{
  char* err;

  _handle = dlopen(_libname.c_str(), RTLD_LAZY);
  if (_handle == nullptr)
    {
      err = dlerror();
      throw (std::string("Can't load library: ") + err);
    }
  _loaded = true;
}

template<typename MODULE>
void* DLLoader<MODULE>::loadSym(const std::string& symbolname)
{
  void* sym;
  char* err;

  if (!_loaded)
    loadLib();
  dlerror();
  sym = dlsym(_handle, symbolname.c_str());
  if (sym == nullptr && (err = dlerror()) == nullptr)
    throw (std::string("Can't get symbol: ") + err);
  return sym;
}

};
};

#endif
