#include <iostream>
#include <thread>
#include "ThreadPool.hpp"

void test()
{
  std::cout << "test thread" << std::endl;
}

int main(int ac, char *av[])
{
//  ThreadPool pool(5);
  std::cout << "Hello from server" << std::endl;

  std::thread t(&test);
  t.join();
  return 0;
}
