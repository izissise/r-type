#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "Server.hpp"

/*
#include "DLLoaderFactory.hpp"
#include "DLManager.hpp"
#include "AMonster.hpp"*/

int main(int ac, char *av[])
{
  std::vector<std::string> args;

/*  DynamicLibrary::DLManager<AMonster> dlManger("createMonster");

  dlManger.loadAllInDirectory("monsters/ExampleMonster/");

  for (auto& i : dlManger.getLoadedModulesNames())
    {
      std::unique_ptr<AMonster> monster = dlManger.createModule(i);
      std::cout << monster->getName() << " " << monster->getPower() << " " << monster->getLife() << std::endl;
    }*/

  for (int acc = 0; acc < ac; ++acc)
    args.push_back(av[acc]);
  try {
      Server serv(args);
      serv.run();
    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  return 0;
}
