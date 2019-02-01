#include <catch.hpp>

#include <mgs/meta/concepts/core/convertible_to.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace core_concepts = concepts::core;

namespace
{
struct A
{
};

struct B
{
  explicit operator A();
};

struct C
{
  operator A();
};
}

void f(int (&f2)[]);

TEST_CASE("ConvertibleTo", "[meta][concepts][core]")
{
  static_assert(core_concepts::is_convertible_to<void, void>::value, "");
  static_assert(
      !core_concepts::is_convertible_to<struct incomplete, void>::value, "");
  static_assert(
      !core_concepts::is_convertible_to<void, struct incomplete>::value, "");

  static_assert(core_concepts::is_convertible_to<int(&&)[], int*>::value, "");
  static_assert(core_concepts::is_convertible_to<int[], int*>::value, "");
  static_assert(core_concepts::is_convertible_to<C, A>::value, "");
  static_assert(!core_concepts::is_convertible_to<B, A>::value, "");
}
