#include <catch.hpp>

#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace core_concepts = concepts::core;

TEST_CASE("CompleteType", "[meta][concepts][core]")
{
  static_assert(core_concepts::is_complete_type<int>::value, "");
  static_assert(core_concepts::is_complete_type<struct incomplete*>::value, "");
  static_assert(!core_concepts::is_complete_type<struct incomplete>::value, "");
  static_assert(!core_concepts::is_complete_type<void>::value, "");

  test_helpers::generate_failed_requirements_tests<core_concepts::is_complete_type<void>>();
}
