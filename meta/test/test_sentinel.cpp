#include <iterator>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/comparison/weakly_equality_comparable_with.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/static_asserts.hpp>

#include "meta_test_helpers.hpp"

#include <mgs/meta/concepts/comparison/strict_totally_ordered.hpp>

using namespace mgs::meta;
namespace iterator_concepts = concepts::iterator;
namespace comparison_concepts = concepts::comparison;
namespace object_concepts = concepts::object;

namespace
{
struct pointer_sentinel
{
};

template <typename T>
bool operator==(T*, pointer_sentinel);
template <typename T>
bool operator!=(T*, pointer_sentinel);
template <typename T>
bool operator==(pointer_sentinel, T*);
template <typename T>
bool operator!=(pointer_sentinel, T*);

struct non_weakly_equality_comparable
{
};

struct non_semiregular
{
  non_semiregular() = delete;
};

template <typename T>
bool operator==(T*, non_semiregular);
template <typename T>
bool operator!=(T*, non_semiregular);
template <typename T>
bool operator==(non_semiregular, T*);
template <typename T>
bool operator!=(non_semiregular, T*);
}

// au

TEST_CASE("Sentinel", "[meta][concepts][iterator]")
{
  static_assert(iterator_concepts::is_sentinel<char*, char*>::value, "");
  static_assert(iterator_concepts::is_sentinel<pointer_sentinel, char*>::value, "");

  static_assert(!iterator_concepts::is_sentinel<non_weakly_equality_comparable, char*>::value, "");
  static_assert(!iterator_concepts::is_sentinel<pointer_sentinel, void*>::value, "");
  static_assert(!iterator_concepts::is_sentinel<non_semiregular, char*>::value, "");

  static_assert(!iterator_concepts::is_sentinel<void, void>::value, "");
  static_assert(!iterator_concepts::is_sentinel<struct incomplete, incomplete>::value, "");

  generate_failed_requirements_tests<
      iterator_concepts::is_sentinel<non_weakly_equality_comparable, char*>,
      std::tuple<comparison_concepts::is_weakly_equality_comparable_with<
          non_weakly_equality_comparable,
          char*>>>();

  generate_failed_requirements_tests<
      iterator_concepts::is_sentinel<pointer_sentinel, void*>,
      std::tuple<
          iterator_concepts::is_iterator<void*>,
          iterator_concepts::is_iterator_traits<std::iterator_traits<void*>>,
          iterator_concepts::is_weakly_incrementable<void*>>>();

  generate_failed_requirements_tests<
      iterator_concepts::is_sentinel<non_semiregular, char*>,
      std::tuple<object_concepts::is_semiregular<non_semiregular>>>();
}
