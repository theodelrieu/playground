#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/core/swappable.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/static_asserts.hpp>

#include "meta_test_helpers.hpp"

using namespace mgs::meta;
namespace object_concepts = concepts::object;
namespace core_concepts = concepts::core;

namespace
{
struct non_default_constructible
{
  non_default_constructible() = delete;
  non_default_constructible(non_default_constructible const&) = default;
  non_default_constructible(non_default_constructible &&) = default;
  non_default_constructible& operator=(non_default_constructible const&) = default;
  non_default_constructible& operator=(non_default_constructible &&) = default;
  ~non_default_constructible() = default;
};

struct non_copy_constructible
{
  non_copy_constructible() = default;
  non_copy_constructible(non_copy_constructible const&) = delete;
  non_copy_constructible(non_copy_constructible &&) = default;
  non_copy_constructible& operator=(non_copy_constructible const&) = default;
  non_copy_constructible& operator=(non_copy_constructible &&) = default;
  ~non_copy_constructible() = default;
};

struct non_move_constructible
{
  non_move_constructible() = default;
  non_move_constructible(non_move_constructible const&) = default;
  non_move_constructible(non_move_constructible &&) = delete;
  non_move_constructible& operator=(non_move_constructible const&) = default;
  non_move_constructible& operator=(non_move_constructible &&) = default;
  ~non_move_constructible() = default;
};

struct non_copy_assignable
{
  non_copy_assignable() = default;
  non_copy_assignable(non_copy_assignable const&) = default;
  non_copy_assignable(non_copy_assignable &&) = default;
  non_copy_assignable& operator=(non_copy_assignable const&) = delete;
  non_copy_assignable& operator=(non_copy_assignable &&) = default;
  ~non_copy_assignable() = default;
};

struct non_move_assignable
{
  non_move_assignable() = default;
  non_move_assignable(non_move_assignable const&) = default;
  non_move_assignable(non_move_assignable &&) = default;
  non_move_assignable& operator=(non_move_assignable const&) = default;
  non_move_assignable& operator=(non_move_assignable &&) = delete;
  ~non_move_assignable() = default;
};

struct non_destructible
{
  non_destructible() = default;
  non_destructible(non_destructible const&) = default;
  non_destructible(non_destructible &&) = default;
  non_destructible& operator=(non_destructible const&) = default;
  non_destructible& operator=(non_destructible &&) = default;
  ~non_destructible() = delete;
};

struct non_swappable
{
};

void swap(non_default_constructible&, non_default_constructible&);
void swap(non_copy_constructible&, non_copy_constructible&);
void swap(non_move_constructible&, non_move_constructible&);
void swap(non_copy_assignable&, non_copy_assignable&);
void swap(non_move_assignable&, non_move_assignable&);
void swap(non_destructible&, non_destructible&);
void swap(non_swappable&, non_swappable&) = delete;

struct correct
{
};
}

TEST_CASE("Semiregular", "[meta][concepts][object]")
{
  static_assert(object_concepts::is_semiregular<int>::value, "");
  static_assert(object_concepts::is_semiregular<correct>::value, "");

  static_assert(!object_concepts::is_semiregular<void>::value, "");
  static_assert(!object_concepts::is_semiregular<struct incomplete>::value, "");

  static_assert(!object_concepts::is_semiregular<non_default_constructible>::value, "");
  static_assert(!object_concepts::is_semiregular<non_copy_constructible>::value, "");
  static_assert(!object_concepts::is_semiregular<non_move_constructible>::value, "");
  static_assert(!object_concepts::is_semiregular<non_copy_assignable>::value, "");
  static_assert(!object_concepts::is_semiregular<non_move_assignable>::value, "");
  static_assert(!object_concepts::is_semiregular<non_destructible>::value, "");
  static_assert(!object_concepts::is_semiregular<non_swappable>::value, "");

  generate_failed_requirements_tests<
      object_concepts::is_semiregular<non_swappable>,
      std::tuple<core_concepts::is_swappable<non_swappable>>>();

  // other types have a swap overload
  generate_failed_requirements_tests<
      object_concepts::is_semiregular<non_destructible>>();
}
