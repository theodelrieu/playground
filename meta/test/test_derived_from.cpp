#include <catch2/catch.hpp>

#include <mgs/meta/concepts/derived_from.hpp>

#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

namespace
{
struct base
{
};

struct derived : base
{
};
}

TEST_CASE("derived_from")
{
  static_assert(is_derived_from<derived, base>::value, "");
  static_assert(is_derived_from<base, base>::value, "");
  static_assert(is_derived_from<derived const volatile, base>::value, "");
  static_assert(is_derived_from<derived, base const volatile>::value, "");
  static_assert(!is_derived_from<base, derived>::value, "");

  static_assert(!is_derived_from<int, int>::value, "");
  static_assert(!is_derived_from<derived, void>::value, "");
  static_assert(!is_derived_from<void, base>::value, "");

  test_helpers::generate_failed_requirements_tests<is_derived_from<int, int>>();
}
