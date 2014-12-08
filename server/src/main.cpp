#include <iostream>
#include <string>
#include <exception>

#include "Server.hpp"

int main(int ac, char *av[])
{
  try {
      Server serv();
    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  return 0;
}