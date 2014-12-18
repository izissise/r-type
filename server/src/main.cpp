#include <iostream>
#include <string>
#include <exception>

#include "Server.hpp"

int main(int, char**)
{
  try {
      Server serv("::1", "8000");
      serv.run();
    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  return 0;
}
