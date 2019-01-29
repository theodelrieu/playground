#include <catch.hpp>

#include <mgs/meta/concepts/core/constructible.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace core_concepts = concepts::core;

namespace
{
struct S
{
  S(int);
  S(S const&) = default;
  S(S&&) = delete;
};
}

TEST_CASE("Constructible", "[meta][concepts][core]")
{
  static_assert(core_concepts::is_constructible<int>::value, "");
  static_assert(core_concepts::is_constructible<struct incomplete*>::value, "");
  static_assert(core_concepts::is_constructible<struct incomplete&, struct incomplete&>::value, "");
  static_assert(core_concepts::is_constructible<S, float>::value, "");
  static_assert(core_concepts::is_constructible<S, S&>::value, "");

  static_assert(!core_concepts::is_constructible<S&, S&&>::value, "");
  static_assert(!core_concepts::is_constructible<S>::value, "");
  static_assert(!core_concepts::is_constructible<struct incomplete>::value, "");
  static_assert(!core_concepts::is_constructible<struct incomplete, int>::value, "");
  static_assert(!core_concepts::is_constructible<void, int>::value, "");
  static_assert(!core_concepts::is_constructible<void>::value, "");
}
