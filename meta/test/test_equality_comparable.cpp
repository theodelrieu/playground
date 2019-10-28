#include <catch2/catch.hpp>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/equality_comparable_with.hpp>
#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

namespace
{
struct fake_sentinel
{
};

struct fake_iterator
{
};

bool operator==(fake_iterator, fake_iterator);
bool operator==(fake_iterator, fake_sentinel);
bool operator==(fake_sentinel, fake_iterator);

bool operator!=(fake_iterator, fake_iterator);
bool operator!=(fake_iterator, fake_sentinel);
bool operator!=(fake_sentinel, fake_iterator);
}

TEST_CASE("weakly_equality_comparable_with")
{
  static_assert(is_weakly_equality_comparable_with<int, float>::value, "");
  static_assert(!is_weakly_equality_comparable_with<int, void>::value, "");
  static_assert(
      is_weakly_equality_comparable_with<fake_iterator, fake_iterator>::value,
      "");
  static_assert(
      is_weakly_equality_comparable_with<fake_iterator, fake_sentinel>::value,
      "");
  static_assert(
      is_weakly_equality_comparable_with<fake_sentinel, fake_iterator>::value,
      "");
  static_assert(
      !is_weakly_equality_comparable_with<fake_sentinel, fake_sentinel>::value,
      "");
  test_helpers::generate_failed_requirements_tests<
      is_weakly_equality_comparable_with<void, void>>();
}

TEST_CASE("equality_comparable")
{
  static_assert(is_equality_comparable<int>::value, "");
  static_assert(!is_equality_comparable<void>::value, "");
  static_assert(is_equality_comparable<fake_iterator>::value, "");
  static_assert(!is_equality_comparable<fake_sentinel>::value, "");

  test_helpers::generate_failed_requirements_tests<
      is_equality_comparable<void>>();
}

TEST_CASE("equality_comparable_with")
{
  static_assert(is_equality_comparable_with<int, float>::value, "");
  static_assert(!is_equality_comparable_with<void, float>::value, "");
  static_assert(
      is_equality_comparable_with<fake_iterator, fake_iterator>::value, "");
  static_assert(
      !is_equality_comparable_with<fake_iterator, fake_sentinel>::value, "");
  static_assert(
      !is_equality_comparable_with<fake_sentinel, fake_iterator>::value, "");
  static_assert(
      !is_equality_comparable_with<fake_sentinel, fake_sentinel>::value, "");

  test_helpers::generate_failed_requirements_tests<
      is_equality_comparable_with<int, fake_iterator>,
      std::tuple<is_weakly_equality_comparable_with<int, fake_iterator>>>();

  test_helpers::generate_failed_requirements_tests<
      is_equality_comparable_with<int, fake_sentinel>,
      std::tuple<is_equality_comparable<fake_sentinel>,

                 is_weakly_equality_comparable_with<int, fake_sentinel>>>();
}
