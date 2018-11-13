#include <cstdlib>
#include <iostream>
#include <string>

#include <mgs/base16.hpp>

int main(int argc, char const* argv[])
{
  auto&& h = "Hello, World!";

  std::cout << mgs::base16::encode(h) << std::endl;
}
