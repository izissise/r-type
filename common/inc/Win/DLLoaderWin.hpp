#ifndef DLLOADERWIN_HPP_
# define DLLOADERWIN_HPP_

# include <exception>
# include <stdexcept>
# include <string>

# include <windows.h>

# include "ADLLoader.hpp"

namespace DynamicLibrary {
namespace Win {

template<typename MODULE>
class DLLoader : public ADLLoader<MODULE>
{

public:
  DLLoader(const std::string& libPath, const std::string& symbolName)
    : ADLLoader<MODULE>(libPath, symbolName), _loaded(false), _handle(nullptr)
  {
    loadLib();
  };

  virtual ~DLLoader()
  {
    if (_loaded && _handle)
      FreeLibrary(_handle);
  }

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
  }

private:
  void* loadSym(const std::string& symbolname)
  {
    void* sym;

    if (!_loaded)
      loadLib();
    sym = GetProcAddress(static_cast<HMODULE>(_handle), symbolname);
    if (sym == nullptr)
      throw std::runtime_error(std::string("Can't get symbol: ") + symbolname);
    return sym;
  };

  void loadLib()
  {
    _handle = LoadLibrary(this->_libName.c_str());
    if (_handle == nullptr)
      throw std::runtime_error(std::string("Can't load library: ") + this->_libName);
    _loaded = true;
  };

private:
  bool  _loaded;
  HINSTANCE _handle;
};
};
};

#endif // !DLLOADERWIN_HPP_
