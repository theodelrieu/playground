#include <iterator>
#include <memory>
#include <vector>

#include <catch.hpp>

#include <mgs/meta/iter_value_t.hpp>
#include <mgs/meta/readable_traits.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

TEST_CASE("ReadableTraits", "[meta]")
{
  static_assert(!is_detected<iter_value_t, void>::value, "");
  static_assert(!is_detected<iter_value_t, struct incomplete>::value, "");
  static_assert(!is_detected<iter_value_t, void*>::value, "");

  static_assert(
      std::is_same<iter_value_t<struct incomplete*>, struct incomplete>::value,
      "");
  static_assert(std::is_same<iter_value_t<std::vector<int>>, int>::value, "");
  static_assert(std::is_same<iter_value_t<std::unique_ptr<int>>, int>::value, "");
}
