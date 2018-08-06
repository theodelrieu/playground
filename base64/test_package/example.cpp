#include <cstdlib>
#include <iostream>
#include <string>

#include <mgs/base64/encoder.hpp>

int main(int argc, char const* argv[])
{
  auto const h = "Hello, World!";
  mgs::base64::encoder<char const*> enc(h, h + std::strlen(h));

  std::cout << std::string(enc.begin(), v.end()) << std::endl;
}
