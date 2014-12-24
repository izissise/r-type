#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "Server.hpp"

#include "FSFactory.hpp"

int main(int ac, char *av[])
{
  std::vector<std::string> args;

  FileSystem::AFSHandler& fs = FileSystem::FSFactory::retrieveFSHandler();
  std::cout << "CurDir: " << fs.currentPath() << std::endl;
  for (auto i : fs.listDir("."))
  {

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
