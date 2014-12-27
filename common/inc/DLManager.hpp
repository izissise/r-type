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

    std::string tmpPath = path + (fs.isPathSlashTerminated(path) ? "" : fs.getPathSeparator());
    for (auto& i : files)
      {
        if (fs.fileExtension(i) == ext)
          {
            loadLib(tmpPath + fs.filePath(i), fs.fileName(i));
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

  std::unique_ptr<MODULE> createModule(const std::string& moduleName) const
  {
    try {
        return (_moduleMap.at(moduleName))->callSymbol();
      }
    catch (std::out_of_range& e)
      {
        throw std::runtime_error("Module " + moduleName + " not loaded");
      }
  };

  std::unique_ptr<MODULE> createModule(const std::string& moduleName, const std::string& symName) const
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

    std::string rModuleName = moduleName;
    if (moduleName.find(getDynLibPrefix()) == 0)
      rModuleName = moduleName.substr(getDynLibPrefix().size());
    try
      {
        _moduleMap.at(rModuleName);
        std::cerr << rModuleName << " already loaded." << std::endl;
        return;
      }
    catch (std::out_of_range& e)
      {}
    try
      {
        _moduleMap[rModuleName] = DLLoaderFactory::createLoader<MODULE>(path + moduleName + getDynLibExtension(), _defaultSym);
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

  static std::string getDynLibPrefix()
  {
#ifdef APPLE
    return "";
#else
# ifdef UNIX
    return "lib";
# endif
#endif
#ifdef WIN32
    return "";
#endif
  };

protected:
  std::string _defaultSym;
  std::map<std::string, std::unique_ptr<ADLLoader<MODULE>>> _moduleMap;
};

};

#endif // DLMANAGER_H
