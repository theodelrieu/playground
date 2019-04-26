#include <catch.hpp>

#include <cstdint>
#include <cstddef>

#include <mgs/config.hpp>
#include <mgs/codecs/concepts/byte.hpp>

using namespace mgs::codecs;

TEST_CASE("Byte", "[concepts]")
{
  static_assert(!is_byte<int>::value, "");
  static_assert(!is_byte<void>::value, "");
  static_assert(!is_byte<struct incomplete>::value, "");

  static_assert(is_byte<signed char>::value, "");
  static_assert(is_byte<char>::value, "");
  static_assert(is_byte<unsigned char>::value, "");
  static_assert(is_byte<unsigned char>::value, "");
  static_assert(is_byte<std::uint8_t>::value, "");
  static_assert(is_byte<std::int8_t>::value, "");

#if MGS_HAS_CPP17
  static_assert(is_byte<std::byte>::value, "");
#endif
}
