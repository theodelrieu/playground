#include <iterator>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/incrementable.hpp>
#include <mgs/meta/concepts/weakly_incrementable.hpp>
#include <mgs/meta/concepts/regular.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

namespace
{
struct non_post_incrementable
{
  using difference_type = std::ptrdiff_t;
  non_post_incrementable& operator++();
};

struct non_pre_incrementable
{
  using difference_type = std::ptrdiff_t;
  void operator++(int);
};

struct weakly_incrementable
{
  using difference_type = std::ptrdiff_t;

  weakly_incrementable& operator++();
  weakly_incrementable operator++(int);
};

struct regular_and_weakly_incrementable
{
  using difference_type = std::ptrdiff_t;

  regular_and_weakly_incrementable& operator++();
  void operator++(int);
};

bool operator==(regular_and_weakly_incrementable,
                regular_and_weakly_incrementable);
bool operator!=(regular_and_weakly_incrementable,
                regular_and_weakly_incrementable);

struct non_regular
{
  using difference_type = std::ptrdiff_t;
  non_regular& operator++();
  non_regular operator++(int);
};

struct incrementable
{
  using difference_type = std::ptrdiff_t;
  incrementable& operator++();
  incrementable operator++(int);
};

bool operator==(incrementable, incrementable);
bool operator!=(incrementable, incrementable);
}

TEST_CASE("WeaklyIncrementable", "[meta][concepts][iterator]")
{
  static_assert(is_weakly_incrementable<char*>::value, "");
  static_assert(is_weakly_incrementable<char>::value, "");
  static_assert(is_weakly_incrementable<weakly_incrementable>::value, "");
  static_assert(
      is_weakly_incrementable<regular_and_weakly_incrementable>::value, "");
  static_assert(
      is_weakly_incrementable<std::back_insert_iterator<std::string>>::value,
      "");

  static_assert(!is_weakly_incrementable<void>::value, "");
  static_assert(!is_weakly_incrementable<struct incomplete>::value, "");
  static_assert(!is_weakly_incrementable<non_post_incrementable>::value, "");
  static_assert(!is_weakly_incrementable<non_pre_incrementable>::value, "");

  test_helpers::generate_failed_requirements_tests<
      is_weakly_incrementable<non_post_incrementable>>();

  test_helpers::generate_failed_requirements_tests<
      is_weakly_incrementable<non_pre_incrementable>>();
}

TEST_CASE("Incrementable", "[meta][concepts][iterator]")
{
  static_assert(is_incrementable<char*>::value, "");
  static_assert(is_incrementable<char>::value, "");
  static_assert(is_incrementable<incrementable>::value, "");

  static_assert(!is_incrementable<non_regular>::value, "");
  static_assert(!is_incrementable<regular_and_weakly_incrementable>::value, "");
  static_assert(!is_incrementable<void>::value, "");
  static_assert(!is_incrementable<struct incomplete>::value, "");
  static_assert(!is_incrementable<non_post_incrementable>::value, "");
  static_assert(!is_incrementable<non_pre_incrementable>::value, "");
  static_assert(!is_incrementable<void*>::value, "");
  static_assert(!is_incrementable<int (*)()>::value, "");

  test_helpers::generate_failed_requirements_tests<
      is_incrementable<non_regular>,
      std::tuple<is_regular<non_regular>,
                 is_equality_comparable<non_regular>>>();

  test_helpers::generate_failed_requirements_tests<
      is_incrementable<non_post_incrementable>,
      std::tuple<is_regular<non_post_incrementable>,
                 is_equality_comparable<non_post_incrementable>,
                 is_weakly_incrementable<non_post_incrementable>>>();

  test_helpers::generate_failed_requirements_tests<
      is_incrementable<regular_and_weakly_incrementable>>();
}
