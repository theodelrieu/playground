
#include <catch.hpp>

#include <mgs/encoders/base32_lazy.hpp>

using namespace std::string_literals;
using namespace mgs;

TEST_CASE("b32 lazy", "[base32]")
{
  auto const clear = "a"s;
  auto const encoded = "ME======"s;

  encoders::base32_lazy_encoder<std::string::const_iterator> enc(clear.begin(),
                                                                 clear.end());

  std::string s(enc.begin(), enc.end());
  CHECK(s == encoded);

}
