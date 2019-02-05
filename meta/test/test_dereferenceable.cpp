#include <memory>

#include <catch.hpp>

#include <mgs/meta/concepts/iterator/dereferenceable.hpp>

#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace iterator_concepts = concepts::iterator;

namespace
{
struct invalid
{
  void operator*();
};
}

TEST_CASE("Dereferenceable", "[meta][concepts][iterator]")
{
  static_assert(!iterator_concepts::is_dereferenceable<char>::value, "");
  static_assert(!iterator_concepts::is_dereferenceable<void>::value, "");
  static_assert(!iterator_concepts::is_dereferenceable<void*>::value, "");
  static_assert(!iterator_concepts::is_dereferenceable<invalid>::value, "");
  static_assert(!iterator_concepts::is_dereferenceable<void() const>::value,
                "");
  static_assert(!iterator_concepts::is_dereferenceable<void()&>::value, "");
  static_assert(!iterator_concepts::is_dereferenceable<void() &&>::value, "");
  static_assert(!iterator_concepts::is_dereferenceable<void() volatile>::value,
                "");
  static_assert(
      !iterator_concepts::is_dereferenceable<void (invalid::*)()>::value, "");

  static_assert(
      !iterator_concepts::is_dereferenceable<std::unique_ptr<void>>::value, "");

  static_assert(
      iterator_concepts::is_dereferenceable<struct incomplete*>::value, "");
  static_assert(iterator_concepts::is_dereferenceable<char*>::value, "");
  static_assert(
      iterator_concepts::is_dereferenceable<std::unique_ptr<char>>::value, "");
  static_assert(iterator_concepts::is_dereferenceable<void()>::value, "");
  static_assert(iterator_concepts::is_dereferenceable<void (*)()>::value, "");
  static_assert(iterator_concepts::is_dereferenceable<void (&)()>::value, "");
}
