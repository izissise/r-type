#include <iostream>

#include "ClientGame.hpp"

int main(int, char **)
{
  try
  {
    ClientGame  game;
    
    game.run();
    return (0);
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
}