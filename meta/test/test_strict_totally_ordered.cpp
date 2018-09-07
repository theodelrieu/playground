#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/comparison/equality_comparable.hpp>
#include <mgs/meta/concepts/comparison/strict_totally_ordered.hpp>
#include <mgs/meta/concepts/comparison/strict_totally_ordered_with.hpp>
#include <mgs/meta/static_asserts.hpp>

#include "meta_test_helpers.hpp"

using namespace mgs::meta;
namespace comparison_concepts = concepts::comparison;

namespace
{
struct almost_correct
{
};

bool operator<(almost_correct, almost_correct);
bool operator>(almost_correct, almost_correct);
bool operator>=(almost_correct, almost_correct);

struct correct
{
};

bool operator==(correct, correct);
bool operator!=(correct, correct);
bool operator<(correct, correct);
bool operator>(correct, correct);
bool operator>=(correct, correct);
bool operator<=(correct, correct);
}

TEST_CASE("StrictTotallyOrdered", "[meta][concepts][comparison]")
{
  static_assert(comparison_concepts::is_strict_totally_ordered<int>::value, "");
  static_assert(comparison_concepts::is_strict_totally_ordered<correct>::value,
                "");
  static_assert(
      !comparison_concepts::is_strict_totally_ordered<almost_correct>::value,
      "");
  static_assert(!comparison_concepts::is_strict_totally_ordered<void>::value,
                "");

  generate_failed_requirements_tests<
      comparison_concepts::is_strict_totally_ordered<almost_correct>>(
      std::tuple<
          comparison_concepts::is_equality_comparable<almost_correct>>{});
}

TEST_CASE("StrictTotallyOrderedWith", "[meta][concepts][comparison]")
{
  static_assert(comparison_concepts::is_strict_totally_ordered_with<int, float>::value, "");
  static_assert(
      comparison_concepts::is_strict_totally_ordered_with<correct,
                                                          correct>::value,
      "");
  static_assert(
      !comparison_concepts::
          is_strict_totally_ordered_with<almost_correct, almost_correct>::value,
      "");
  static_assert(
      !comparison_concepts::is_strict_totally_ordered_with<void, void>::value,
      "");

  generate_failed_requirements_tests<
      comparison_concepts::is_strict_totally_ordered_with<almost_correct, int>>(
      std::tuple<
          comparison_concepts::is_strict_totally_ordered<almost_correct>,
          comparison_concepts::is_equality_comparable<almost_correct>>{});
}
