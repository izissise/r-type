#include <iostream>
#include <thread>
#include <ThreadPool.hpp>

void test() {
//  std::cout << << std::endl;
}

int main(int ac, char *av[])
{
  ThreadPool pool(5);
  std::cout << "Hello from server" << std::endl;

  return 0;
}