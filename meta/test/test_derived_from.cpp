#include <catch.hpp>

#include <mgs/meta/concepts/core/derived_from.hpp>

#include <mgs/meta/static_asserts.hpp>

#include "meta_test_helpers.hpp"

using namespace mgs::meta;
namespace core_concepts = concepts::core;

namespace
{
struct base
{
};

struct derived : base
{
};
}

TEST_CASE("DerivedFrom", "[meta][concepts][core]")
{
  SECTION("trait")
  {
    static_assert(core_concepts::is_derived_from<derived, base>::value, "");
    static_assert(core_concepts::is_derived_from<base, base>::value, "");
    static_assert(core_concepts::is_derived_from<derived const volatile, base>::value, "");
    static_assert(core_concepts::is_derived_from<derived, base const volatile>::value, "");
    static_assert(!core_concepts::is_derived_from<base, derived>::value, "");

    static_assert(!core_concepts::is_derived_from<int, int>::value, "");
    static_assert(!core_concepts::is_derived_from<derived, void>::value, "");
    static_assert(!core_concepts::is_derived_from<void, base>::value, "");
  }

  generate_failed_requirements_tests<core_concepts::is_derived_from<int, int>>(
      std::tuple<>{});
}
