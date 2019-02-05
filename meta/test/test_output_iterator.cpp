#include <iterator>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/iterator/dereferenceable.hpp>
#include <mgs/meta/concepts/iterator/output_iterator.hpp>
#include <mgs/meta/concepts/iterator/weakly_incrementable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace iterator_concepts = concepts::iterator;

namespace
{
struct output_iterator
{
  output_iterator& operator++();
  output_iterator& operator++(int);
  output_iterator& operator*();

  template <typename T>
  output_iterator& operator=(T&&);
};

struct non_assignable_iterator
{
  non_assignable_iterator& operator++();
  non_assignable_iterator& operator++(int);
  non_assignable_iterator& operator*();
};
}

TEST_CASE("OutputIterator", "[meta][concepts][iterator]")
{
  static_assert(iterator_concepts::is_output_iterator<char*, char>::value, "");
  static_assert(
      iterator_concepts::is_output_iterator<output_iterator, int>::value, "");

  static_assert(!iterator_concepts::is_output_iterator<non_assignable_iterator,
                                                       int>::value,
                "");
  static_assert(!iterator_concepts::is_output_iterator<void, int>::value, "");
  static_assert(!iterator_concepts::is_output_iterator<void*, int>::value, "");
  static_assert(
      !iterator_concepts::is_output_iterator<struct incomplete, int>::value,
      "");
  static_assert(
      !iterator_concepts::is_output_iterator<struct incomplete*, int>::value,
      "");
  static_assert(
      !iterator_concepts::is_output_iterator<struct incomplete*,
                                             struct incomplete&>::value,
      "");
}
