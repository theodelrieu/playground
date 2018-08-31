#include <catch.hpp>

#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/core/swappable.hpp>

#include <mgs/meta/static_asserts.hpp>

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

template <typename Requirement>
void generate_static_asserts_tests()
{
  SECTION("requirements")
  {
    using expected_failed_requirements =
        std::tuple<Requirement>;
    using failed_requirements =
        typename detail::collect_failed_requirements<Requirement>::type;

    static_assert(
        std::is_same<expected_failed_requirements, failed_requirements>::value,
        "");
  }

  SECTION("static_asserts")
  {
    using expected_failed_static_asserts =
        std::tuple<typename Requirement::static_assert_t>;
    using failed_static_asserts =
        typename detail::collect_static_asserts<Requirement>::type;

    static_assert(std::is_same<expected_failed_static_asserts,
                               failed_static_asserts>::value,
                  "");
  }
}
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
  }

  generate_static_asserts_tests<core_concepts::is_derived_from<int, int>>();
}
