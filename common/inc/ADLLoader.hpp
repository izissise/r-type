#ifndef IDLLOADER_HPP_INCLUDED
# define IDLLOADER_HPP_INCLUDED

# include <memory>

namespace DynamicLibrary
{

template<typename MODULE>
class ADLLoader
{
public:
  ADLLoader(const std::string& libPath, const std::string& symbolName)
    : _libname(libPath), _symbolname(symbolName)
  {};
  virtual ~ADLLoader() = default;

  //template<typename ... Args>
  virtual std::unique_ptr<MODULE> callSymbol(/*Args&& ... args*/) const = 0;

protected:
  std::string _libname;
  std::string _symbolname;
};
};

#endif // IDLLOADER_HPP_INCLUDED
