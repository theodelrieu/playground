#include <iterator>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/comparison/equality_comparable.hpp>
#include <mgs/meta/concepts/iterator/incrementable.hpp>
#include <mgs/meta/concepts/iterator/weakly_incrementable.hpp>
#include <mgs/meta/concepts/object/regular.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/static_asserts.hpp>

#include "meta_test_helpers.hpp"

using namespace mgs::meta;
namespace iterator_concepts = concepts::iterator;
namespace comparison_concepts = concepts::comparison;
namespace object_concepts = concepts::object;

namespace
{
struct non_post_incrementable
{
  non_post_incrementable& operator++();
};

struct non_pre_incrementable
{
  void operator++(int);
};

struct non_semiregular
{
  non_semiregular() = delete;

  non_semiregular& operator++();
  void operator++(int);
};

struct weakly_incrementable
{
  weakly_incrementable& operator++();
  void operator++(int);
};

struct regular_and_weakly_incrementable
{
  regular_and_weakly_incrementable& operator++();
  void operator++(int);
};

bool operator==(regular_and_weakly_incrementable, regular_and_weakly_incrementable);
bool operator!=(regular_and_weakly_incrementable, regular_and_weakly_incrementable);

struct non_regular
{
  non_regular& operator++();
  non_regular operator++(int);
};

struct incrementable
{
  incrementable& operator++();
  incrementable operator++(int);
};

bool operator==(incrementable, incrementable);
bool operator!=(incrementable, incrementable);
}

TEST_CASE("WeaklyIncrementable", "[meta][concepts][iterator]")
{
  static_assert(iterator_concepts::is_weakly_incrementable<char*>::value, "");
  static_assert(iterator_concepts::is_weakly_incrementable<char>::value, "");
  static_assert(iterator_concepts::is_weakly_incrementable<weakly_incrementable>::value, "");
  static_assert(iterator_concepts::is_weakly_incrementable<regular_and_weakly_incrementable>::value, "");

  static_assert(!iterator_concepts::is_weakly_incrementable<void>::value, "");
  static_assert(!iterator_concepts::is_weakly_incrementable<struct incomplete>::value, "");
  static_assert(!iterator_concepts::is_weakly_incrementable<non_post_incrementable>::value, "");
  static_assert(!iterator_concepts::is_weakly_incrementable<non_pre_incrementable>::value, "");
  static_assert(!iterator_concepts::is_weakly_incrementable<non_semiregular>::value, "");

  generate_failed_requirements_tests<
      iterator_concepts::is_weakly_incrementable<non_semiregular>,
      std::tuple<object_concepts::is_semiregular<non_semiregular>>>();

  generate_failed_requirements_tests<
      iterator_concepts::is_weakly_incrementable<non_post_incrementable>>();

  generate_failed_requirements_tests<
      iterator_concepts::is_weakly_incrementable<non_pre_incrementable>>();
}

TEST_CASE("Incrementable", "[meta][concepts][iterator]")
{
  static_assert(iterator_concepts::is_incrementable<char*>::value, "");
  static_assert(iterator_concepts::is_incrementable<char>::value, "");
  static_assert(iterator_concepts::is_incrementable<incrementable>::value, "");

  static_assert(!iterator_concepts::is_incrementable<non_regular>::value, "");
  static_assert(!iterator_concepts::is_incrementable<regular_and_weakly_incrementable>::value, "");
  static_assert(!iterator_concepts::is_incrementable<void>::value, "");
  static_assert(!iterator_concepts::is_incrementable<struct incomplete>::value, "");
  static_assert(!iterator_concepts::is_incrementable<non_post_incrementable>::value, "");
  static_assert(!iterator_concepts::is_incrementable<non_pre_incrementable>::value, "");
  static_assert(!iterator_concepts::is_incrementable<non_semiregular>::value, "");
  static_assert(!iterator_concepts::is_incrementable<void*>::value, "");
  static_assert(!iterator_concepts::is_incrementable<int (*)()>::value, "");

  generate_failed_requirements_tests<
      iterator_concepts::is_incrementable<non_regular>,
      std::tuple<object_concepts::is_regular<non_regular>,
                 comparison_concepts::is_equality_comparable<non_regular>>>();

  generate_failed_requirements_tests<
      iterator_concepts::is_incrementable<non_post_incrementable>,
      std::tuple<
          object_concepts::is_regular<non_post_incrementable>,
          comparison_concepts::is_equality_comparable<non_post_incrementable>,
          iterator_concepts::is_weakly_incrementable<
              non_post_incrementable>>>();

  generate_failed_requirements_tests<
      iterator_concepts::is_incrementable<regular_and_weakly_incrementable>>();
}
