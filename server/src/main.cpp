#include <iostream>
#include <string>
#include <exception>

#include "Server.hpp"

int main(int ac, char *av[])
{
  (void)ac;
  (void)av;
  try {
      Server serv("::1", "9632");
      serv.run();
    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  return 0;
}
