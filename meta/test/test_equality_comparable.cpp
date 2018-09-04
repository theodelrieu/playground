#include <catch.hpp>

#include <mgs/meta/concepts/comparison/equality_comparable.hpp>
#include <mgs/meta/concepts/comparison/equality_comparable_with.hpp>
#include <mgs/meta/concepts/comparison/weakly_equality_comparable_with.hpp>
#include <mgs/meta/static_asserts.hpp>

#include "meta_test_helpers.hpp"

using namespace mgs::meta;
namespace comparison_concepts = concepts::comparison;

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

TEST_CASE("WeaklyEqualityComparableWith", "[meta][concepts][comparison]")
{
  static_assert(
      comparison_concepts::is_weakly_equality_comparable_with<int,
                                                              float>::value,
      "");
  static_assert(
      !comparison_concepts::is_weakly_equality_comparable_with<int,
                                                               void>::value,
      "");
  static_assert(comparison_concepts::is_weakly_equality_comparable_with<
                    fake_iterator,
                    fake_iterator>::value,
                "");
  static_assert(comparison_concepts::is_weakly_equality_comparable_with<
                    fake_iterator,
                    fake_sentinel>::value,
                "");
  static_assert(comparison_concepts::is_weakly_equality_comparable_with<
                    fake_sentinel,
                    fake_iterator>::value,
                "");
  static_assert(!comparison_concepts::is_weakly_equality_comparable_with<
                    fake_sentinel,
                    fake_sentinel>::value,
                "");
  generate_failed_requirements_tests<
      comparison_concepts::is_weakly_equality_comparable_with<void, void>>(
      std::tuple<>{});
}

TEST_CASE("EqualityComparable", "[meta][concepts][comparison]")
{
  static_assert(comparison_concepts::is_equality_comparable<int>::value, "");
  static_assert(!comparison_concepts::is_equality_comparable<void>::value, "");
  static_assert(
      comparison_concepts::is_equality_comparable<fake_iterator>::value, "");
  static_assert(
      !comparison_concepts::is_equality_comparable<fake_sentinel>::value, "");

  generate_failed_requirements_tests<
      comparison_concepts::is_equality_comparable<void>>(
      std::tuple<comparison_concepts::
                     is_weakly_equality_comparable_with<void, void>>{});
}

TEST_CASE("EqualityComparableWith", "[meta][concepts][comparison]")
{
  static_assert(
      comparison_concepts::is_equality_comparable_with<int, float>::value, "");
  static_assert(
      !comparison_concepts::is_equality_comparable_with<void, float>::value,
      "");
  static_assert(
      comparison_concepts::is_equality_comparable_with<fake_iterator,
                                                       fake_iterator>::value,
      "");
  static_assert(
      !comparison_concepts::is_equality_comparable_with<fake_iterator,
                                                        fake_sentinel>::value,
      "");
  static_assert(
      !comparison_concepts::is_equality_comparable_with<fake_sentinel,
                                                        fake_iterator>::value,
      "");
  static_assert(
      !comparison_concepts::is_equality_comparable_with<fake_sentinel,
                                                        fake_sentinel>::value,
      "");

  generate_failed_requirements_tests<
      comparison_concepts::is_equality_comparable_with<int, fake_iterator>>(
      std::tuple<comparison_concepts::
                     is_weakly_equality_comparable_with<int, fake_iterator>>{});

  generate_failed_requirements_tests<
      comparison_concepts::is_equality_comparable_with<int, fake_sentinel>>(
      std::tuple<comparison_concepts::is_equality_comparable<fake_sentinel>,
                 comparison_concepts::is_weakly_equality_comparable_with<
                     fake_sentinel,
                     fake_sentinel>,
                 comparison_concepts::
                     is_weakly_equality_comparable_with<int, fake_sentinel>>{});
}
