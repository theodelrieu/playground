#include <string>

#include <catch.hpp>

#include <mgs/meta/concepts/core/assignable.hpp>
#include <mgs/meta/concepts/core/constructible.hpp>
#include <mgs/meta/concepts/core/copy_constructible.hpp>
#include <mgs/meta/concepts/core/move_constructible.hpp>
#include <mgs/meta/concepts/object/copyable.hpp>
#include <mgs/meta/concepts/object/movable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace core_concepts = concepts::core;
namespace object_concepts = concepts::object;

namespace
{
struct non_move_constructible
{
  non_move_constructible(int);
  non_move_constructible(non_move_constructible const&) = default;
  non_move_constructible(non_move_constructible&&) = delete;
};

struct non_copy_constructible
{
  non_copy_constructible(non_copy_constructible const&) = delete;
};

struct invalid_assignment_return
{
  invalid_assignment_return operator=(invalid_assignment_return const&);
};
}

TEST_CASE("Constructible", "[meta][concepts][core]")
{
  static_assert(core_concepts::is_constructible<int>::value, "");
  static_assert(core_concepts::is_constructible<struct incomplete*>::value, "");
  static_assert(core_concepts::is_constructible<struct incomplete&,
                                                struct incomplete&>::value,
                "");
  static_assert(
      core_concepts::is_constructible<non_move_constructible, float>::value,
      "");
  static_assert(core_concepts::is_constructible<non_move_constructible,
                                                non_move_constructible&>::value,
                "");

  static_assert(
      !core_concepts::is_constructible<non_move_constructible&,
                                       non_move_constructible&&>::value,
      "");
  static_assert(!core_concepts::is_constructible<non_move_constructible>::value,
                "");
  static_assert(!core_concepts::is_constructible<struct incomplete>::value, "");
  static_assert(!core_concepts::is_constructible<struct incomplete, int>::value,
                "");
  static_assert(!core_concepts::is_constructible<void, int>::value, "");
  static_assert(!core_concepts::is_constructible<void>::value, "");
}

TEST_CASE("Assignable", "[meta][concepts][core]")
{
  static_assert(core_concepts::is_assignable<int&, int>::value, "");

  static_assert(!core_concepts::is_assignable<void, void>::value, "");
  static_assert(!core_concepts::is_assignable<struct incomplete&,
                                              struct incomplete>::value,
                "");

  static_assert(!core_concepts::is_assignable<invalid_assignment_return&,
                                              invalid_assignment_return>::value,
                "");
  static_assert(!core_concepts::is_assignable<int, int>::value, "");
  static_assert(!core_concepts::is_assignable<int&&, int>::value, "");
  static_assert(!core_concepts::is_assignable<int const&, int>::value, "");
}

TEST_CASE("MoveConstructible", "[meta][concepts][core]")
{
  static_assert(core_concepts::is_move_constructible<int>::value, "");

  static_assert(core_concepts::is_move_constructible<char(&)[]>::value, "");
  static_assert(!core_concepts::is_move_constructible<char[]>::value, "");
  static_assert(
      !core_concepts::is_move_constructible<non_move_constructible>::value, "");
  static_assert(!core_concepts::is_move_constructible<void>::value, "");
  static_assert(!core_concepts::is_move_constructible<struct incomplete>::value,
                "");
}

TEST_CASE("CopyConstructible", "[meta][concepts][core]")
{
  static_assert(core_concepts::is_copy_constructible<int>::value, "");

  static_assert(core_concepts::is_copy_constructible<char(&)[]>::value, "");
  static_assert(!core_concepts::is_copy_constructible<char[]>::value, "");
  static_assert(
      !core_concepts::is_copy_constructible<non_move_constructible>::value, "");
  static_assert(
      !core_concepts::is_copy_constructible<non_copy_constructible>::value, "");
  static_assert(!core_concepts::is_copy_constructible<void>::value, "");
  static_assert(!core_concepts::is_copy_constructible<struct incomplete>::value,
                "");
}

TEST_CASE("Movable", "[meta][concepts][object]")
{
  static_assert(object_concepts::is_movable<int>::value, "");
  static_assert(object_concepts::is_movable<std::string>::value, "");

  static_assert(!object_concepts::is_movable<int&>::value, "");
  static_assert(!object_concepts::is_movable<void>::value, "");
  static_assert(!object_concepts::is_movable<struct incomplete>::value, "");
  static_assert(!object_concepts::is_movable<void()>::value, "");
}

TEST_CASE("Copyable", "[meta][concepts][object]")
{
  static_assert(object_concepts::is_copyable<int>::value, "");
  static_assert(object_concepts::is_copyable<std::string>::value, "");

  static_assert(!object_concepts::is_copyable<int&>::value, "");
  static_assert(!object_concepts::is_copyable<void>::value, "");
  static_assert(!object_concepts::is_copyable<struct incomplete>::value, "");
  static_assert(!object_concepts::is_copyable<void()>::value, "");
}
