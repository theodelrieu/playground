#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/strict_totally_ordered.hpp>
#include <mgs/meta/concepts/strict_totally_ordered_with.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
using namespace mgs::meta::concepts;

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
  static_assert(is_strict_totally_ordered<int>::value, "");
  static_assert(is_strict_totally_ordered<correct>::value, "");
  static_assert(!is_strict_totally_ordered<almost_correct>::value, "");
  static_assert(!is_strict_totally_ordered<void>::value, "");

  test_helpers::generate_failed_requirements_tests<
      is_strict_totally_ordered<almost_correct>,
      std::tuple<is_equality_comparable<almost_correct>>>();
}

TEST_CASE("StrictTotallyOrderedWith", "[meta][concepts][comparison]")
{
  static_assert(is_strict_totally_ordered_with<int, float>::value, "");
  static_assert(is_strict_totally_ordered_with<correct, correct>::value, "");
  static_assert(
      !is_strict_totally_ordered_with<almost_correct, almost_correct>::value,
      "");
  static_assert(!is_strict_totally_ordered_with<void, void>::value, "");

  test_helpers::generate_failed_requirements_tests<
      is_strict_totally_ordered_with<almost_correct, int>,
      std::tuple<is_strict_totally_ordered<almost_correct>,
                 is_equality_comparable<almost_correct>>>();
}
