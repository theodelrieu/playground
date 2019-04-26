#include <memory>

#include <catch.hpp>

#include <mgs/meta/concepts/dereferenceable.hpp>

#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

namespace
{
struct invalid
{
  void operator*();
};
}

TEST_CASE("Dereferenceable", "[meta][concepts][iterator]")
{
  static_assert(!is_dereferenceable<char>::value, "");
  static_assert(!is_dereferenceable<void>::value, "");
  static_assert(!is_dereferenceable<void*>::value, "");
  static_assert(!is_dereferenceable<invalid>::value, "");
  static_assert(!is_dereferenceable<void() const>::value, "");
  static_assert(!is_dereferenceable<void()&>::value, "");
  static_assert(!is_dereferenceable<void() &&>::value, "");
  static_assert(!is_dereferenceable<void() volatile>::value, "");
  static_assert(!is_dereferenceable<void (invalid::*)()>::value, "");

  static_assert(!is_dereferenceable<std::unique_ptr<void>>::value, "");

  static_assert(is_dereferenceable<struct incomplete*>::value, "");
  static_assert(is_dereferenceable<char*>::value, "");
  static_assert(is_dereferenceable<std::unique_ptr<char>>::value, "");
  static_assert(is_dereferenceable<void()>::value, "");
  static_assert(is_dereferenceable<void (*)()>::value, "");
  static_assert(is_dereferenceable<void (&)()>::value, "");
}
