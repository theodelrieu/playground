#include <iterator>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/iter_traits.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

namespace
{
struct invalid_iterator
{
};
}

namespace std
{
template <>
struct iterator_traits<invalid_iterator>
{
  using value_type = void;
};
}

TEST_CASE("iter_traits", "[meta]")
{
  static_assert(
      std::is_same<iter_traits<char*>, std::iterator_traits<char*>>::value, "");

  static_assert(std::is_same<iter_traits<void>, void>::value, "");
  static_assert(std::is_same<iter_traits<struct incomplete>, struct incomplete>::value, "");
  static_assert(std::is_same<iter_traits<struct incomplete*>,
                             std::iterator_traits<struct incomplete*>>::value,
                "");
  static_assert(!is_detected<iter_traits, invalid_iterator>::value, "");
}