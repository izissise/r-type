#ifndef ADLLOADER_HPP_INCLUDED
# define ADLLOADER_HPP_INCLUDED

# include <memory>

namespace DynamicLibrary
{

template<typename MODULE>
class ADLLoader
{
public:
  ADLLoader(const std::string& libPath, const std::string& symbolName)
    : _libName(libPath), _symbolName(symbolName)
  {};
  virtual ~ADLLoader() = default;

  //template<typename ... Args>
  virtual std::unique_ptr<MODULE> callSymbol(/*Args&& ... args*/) = 0;

  const std::string& getLibName() const {return _libName;};
  const std::string& getSymName() const {return _symbolName;};
  void setSymName(const std::string& sym) {_symbolName = sym;};

protected:
  std::string _libName;
  std::string _symbolName;
};
};

#endif // IDLLOADER_HPP_INCLUDED
