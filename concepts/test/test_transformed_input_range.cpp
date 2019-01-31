#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch.hpp>

#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs;

namespace object_concepts = mgs::meta::concepts::object;
namespace iterator_concepts = mgs::meta::concepts::iterator;

namespace
{
struct valid_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  valid_range() = default;
  valid_range(underlying_iterator, underlying_sentinel);

  iterator begin();
  iterator end();
};
}

TEST_CASE("TransformedInputRange", "[concepts]")
{
  static_assert(concepts::is_transformed_input_range<valid_range>::value, "");

  static_assert(!concepts::is_transformed_input_range<struct incomplete>::value, "");
  static_assert(!concepts::is_transformed_input_range<void>::value, "");
  static_assert(!concepts::is_transformed_input_range<void*>::value, "");
}
