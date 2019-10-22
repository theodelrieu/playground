#include <string>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/assignable_from.hpp>
#include <mgs/meta/concepts/constructible_from.hpp>
#include <mgs/meta/concepts/copy_constructible.hpp>
#include <mgs/meta/concepts/move_constructible.hpp>
#include <mgs/meta/concepts/copyable.hpp>
#include <mgs/meta/concepts/movable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

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

TEST_CASE("constructible_from")
{
  static_assert(is_constructible_from<int>::value, "");
  static_assert(is_constructible_from<struct incomplete*>::value, "");
  static_assert(
      is_constructible_from<struct incomplete&, struct incomplete&>::value, "");
  static_assert(is_constructible_from<non_move_constructible, float>::value,
                "");
  static_assert(is_constructible_from<non_move_constructible,
                                      non_move_constructible&>::value,
                "");

  static_assert(!is_constructible_from<non_move_constructible&,
                                       non_move_constructible&&>::value,
                "");
  static_assert(!is_constructible_from<non_move_constructible>::value, "");
  static_assert(!is_constructible_from<struct incomplete>::value, "");
  static_assert(!is_constructible_from<struct incomplete, int>::value, "");
  static_assert(!is_constructible_from<void, int>::value, "");
  static_assert(!is_constructible_from<void>::value, "");
}

TEST_CASE("assignable_from")
{
  static_assert(is_assignable_from<int&, int>::value, "");

  static_assert(!is_assignable_from<void, void>::value, "");
  static_assert(
      !is_assignable_from<struct incomplete&, struct incomplete>::value, "");

  static_assert(!is_assignable_from<invalid_assignment_return&,
                                    invalid_assignment_return>::value,
                "");
  static_assert(!is_assignable_from<int, int>::value, "");
  static_assert(!is_assignable_from<int&&, int>::value, "");
  static_assert(!is_assignable_from<int const&, int>::value, "");
}

TEST_CASE("MoveConstructible", "[meta][concepts][core]")
{
  static_assert(is_move_constructible<int>::value, "");

  static_assert(is_move_constructible<char(&)[]>::value, "");
  static_assert(!is_move_constructible<char[]>::value, "");
  static_assert(!is_move_constructible<non_move_constructible>::value, "");
  static_assert(!is_move_constructible<void>::value, "");
  static_assert(!is_move_constructible<struct incomplete>::value, "");
}

TEST_CASE("CopyConstructible", "[meta][concepts][core]")
{
  static_assert(is_copy_constructible<int>::value, "");

  static_assert(is_copy_constructible<char(&)[]>::value, "");
  static_assert(!is_copy_constructible<char[]>::value, "");
  static_assert(!is_copy_constructible<non_move_constructible>::value, "");
  static_assert(!is_copy_constructible<non_copy_constructible>::value, "");
  static_assert(!is_copy_constructible<void>::value, "");
  static_assert(!is_copy_constructible<struct incomplete>::value, "");
}

TEST_CASE("Movable", "[meta][concepts][object]")
{
  static_assert(is_movable<int>::value, "");
  static_assert(is_movable<std::string>::value, "");

  static_assert(!is_movable<int&>::value, "");
  static_assert(!is_movable<void>::value, "");
  static_assert(!is_movable<struct incomplete>::value, "");
  static_assert(!is_movable<void()>::value, "");
}

TEST_CASE("Copyable", "[meta][concepts][object]")
{
  static_assert(is_copyable<int>::value, "");
  static_assert(is_copyable<std::string>::value, "");

  static_assert(!is_copyable<int&>::value, "");
  static_assert(!is_copyable<void>::value, "");
  static_assert(!is_copyable<struct incomplete>::value, "");
  static_assert(!is_copyable<void()>::value, "");
}
