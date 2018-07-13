#include <cstdlib>
#include <iostream>
#include <string>

#include <mgs/detail/base16/encoder.hpp>

int main(int argc, char const *argv[])
{
  auto const h = "Hello, World!";
  mgs::detail::base16_encoder<char const*> enc(h, h + std::strlen(h));

  std::cout << std::string(enc.begin(), v.end()) << std::endl;
}
