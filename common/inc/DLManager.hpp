#ifndef DLMANAGER_H
#define DLMANAGER_H

#include <memory>
#include <exception>
#include <string>
#include <map>
#include <list>
#include <vector>

#include "RTypeConfig.h"

#include "AFSHandler.hpp"
#include "FSFactory.hpp"
#include "ADLLoader.hpp"
#include "DLLoaderFactory.hpp"

namespace DynamicLibrary
{

template<typename MODULE>
class DLManager
{
public:
  DLManager(const std::string& defSym) : _defaultSym(defSym) {};
  virtual ~DLManager() = default;

  void loadAllInDirectory(const std::string& path)
  {
    FileSystem::AFSHandler& fs = FileSystem::FSFactory::retrieveFSHandler();
    std::string ext = getDynLibExtension();
    auto files = fs.listDir(path);

    for (auto& i : files)
      {
        if (fs.fileExtension(i) == ext)
          {
            loadLib(path + fs.filePath(i), fs.fileName(i));
          }
      }
  };

  std::vector<std::string> getLoadedModulesNames() const
  {
    std::vector<std::string> res;

    for (auto it = _moduleMap.begin(), end = _moduleMap.end(); it != end; ++it)
      {
        res.push_back(it->first);
      }
    return res;
  };

  std::unique_ptr<MODULE> createModule(const std::string& moduleName)
  {
    try {
        return (_moduleMap.at(moduleName))->callSymbol();
      }
    catch (std::out_of_range& e)
      {
        throw std::runtime_error("Module " + moduleName + " not loaded");
      }
  };

  std::unique_ptr<MODULE> createModule(const std::string& moduleName, const std::string& symName)
  {
    std::string tmpDefault;
    try {
        tmpDefault = _moduleMap.at(moduleName)->getSymName;
      }
    catch (std::out_of_range& e)
      {
        throw std::runtime_error("Module " + moduleName + " not loaded");
      }
    std::unique_ptr<MODULE> res = createModule(moduleName);
    _moduleMap.at(moduleName)->setSymName(tmpDefault);
    return res;
  }

  void loadLib(const std::string& path, const std::string& moduleName)
  {
    try
      {
        _moduleMap.at(moduleName);
        std::cerr << moduleName << " already loaded." << std::endl;
        return;
      }
    catch (std::out_of_range& e)
      {}
    try
      {
        _moduleMap[moduleName] = DLLoaderFactory::createLoader<MODULE>(path + moduleName + getDynLibExtension(), _defaultSym);
      }
    catch (std::runtime_error& e)
      {
        std::cerr << e.what() << std::endl;
      }
  };

protected:
  static std::string getDynLibExtension()
  {
#ifdef APPLE
    return ".dylib";
#else
# ifdef UNIX
    return ".so";
# endif
#endif
#ifdef WIN32
    return ".dll";
#endif
  };

protected:
  std::string _defaultSym;
  std::map<std::string, std::unique_ptr<ADLLoader<MODULE>>> _moduleMap;
};

};

#endif // DLMANAGER_H
