#include <catch.hpp>

#include <mgs/meta/concepts/core/constructible.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace core_concepts = concepts::core;

TEST_CASE("Constructible", "[meta][concepts][core]")
{
  static_assert(core_concepts::is_constructible<int>::value, "");
  static_assert(core_concepts::is_constructible<struct incomplete*>::value, "");
  static_assert(core_concepts::is_constructible<struct incomplete&, struct incomplete&>::value, "");

  static_assert(!core_concepts::is_constructible<struct incomplete>::value, "");
  static_assert(!core_concepts::is_constructible<void, int>::value, "");
  static_assert(!core_concepts::is_constructible<void>::value, "");
}
