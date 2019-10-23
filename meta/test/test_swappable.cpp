#include <catch2/catch.hpp>

#include <mgs/meta/concepts/swappable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

namespace
{
struct not_swappable
{
  not_swappable(not_swappable const&) = delete;
};

struct adl_swappable
{
  adl_swappable(adl_swappable const&) = delete;
};

void swap(adl_swappable&, adl_swappable&);

template <std::size_t N>
void swap(adl_swappable (&)[N], adl_swappable (&)[N]);
}

TEST_CASE("swappable", "[meta][concepts][core]")
{
  static_assert(is_swappable<int>::value, "");
  static_assert(is_swappable<adl_swappable>::value, "");
  static_assert(is_swappable<int[4]>::value, "");
  static_assert(is_swappable<int(*)[4]>::value, "");
  static_assert(is_swappable<int(&)[4]>::value, "");
  static_assert(is_swappable<adl_swappable[4]>::value, "");
  static_assert(is_swappable<adl_swappable(*)[4]>::value, "");
  static_assert(is_swappable<adl_swappable(&)[4]>::value, "");

  static_assert(!is_swappable<void>::value, "");
  static_assert(!is_swappable<not_swappable>::value, "");

  test_helpers::generate_failed_requirements_tests<is_swappable<void>>();
}
