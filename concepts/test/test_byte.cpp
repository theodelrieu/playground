#include <catch.hpp>

#include <cstdint>
#include <cstddef>

#include <mgs/concepts/byte.hpp>

using namespace mgs;

TEST_CASE("Byte", "[concepts]")
{
  static_assert(!concepts::is_byte<int>::value, "");
  static_assert(!concepts::is_byte<void>::value, "");
  static_assert(!concepts::is_byte<struct incomplete>::value, "");

  static_assert(concepts::is_byte<signed char>::value, "");
  static_assert(concepts::is_byte<char>::value, "");
  static_assert(concepts::is_byte<unsigned char>::value, "");
  static_assert(concepts::is_byte<unsigned char>::value, "");
  static_assert(concepts::is_byte<std::uint8_t>::value, "");
  static_assert(concepts::is_byte<std::int8_t>::value, "");

#if __cplusplus >= 201703L
  static_assert(concepts::is_byte<std::byte>::value, "");
#endif
}
