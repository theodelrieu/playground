#include <iterator>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/iterator_traits.hpp>
#include <mgs/meta/concepts/iterator/weakly_incrementable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace iterator_concepts = concepts::iterator;

namespace
{
struct output_iterator
{
  using value_type = void;
  using pointer = void;
  using reference = void;
  using difference_type = void;
  using iterator_category = std::output_iterator_tag;

  output_iterator& operator++();
  void operator++(int);
  void operator*();
};

struct non_dereferencable_iterator
{
  using value_type = void;
  using pointer = void;
  using reference = void;
  using difference_type = void;
  using iterator_category = std::output_iterator_tag;

  non_dereferencable_iterator& operator++();
  non_dereferencable_iterator& operator++(int);
};

struct non_weakly_incrementable_iterator
{
  using value_type = void;
  using pointer = void;
  using reference = void;
  using difference_type = void;
  using iterator_category = std::output_iterator_tag;

  non_weakly_incrementable_iterator& operator*();
};
}

TEST_CASE("Iterator", "[meta][concepts][iterator]")
{
  static_assert(iterator_concepts::is_iterator<char*>::value, "");
  static_assert(iterator_concepts::is_iterator<output_iterator>::value, "");

  static_assert(!iterator_concepts::is_iterator<char>::value, "");
  static_assert(!iterator_concepts::is_iterator<void>::value, "");
  static_assert(!iterator_concepts::is_iterator<struct incomplete>::value, "");
  static_assert(!iterator_concepts::is_iterator<void*>::value, "");

  static_assert(!iterator_concepts::is_iterator<non_dereferencable_iterator>::value, "");
  static_assert(!iterator_concepts::is_iterator<non_weakly_incrementable_iterator>::value, "");

  test_helpers::generate_failed_requirements_tests<
      iterator_concepts::is_iterator<int>,
      std::tuple<
          iterator_concepts::is_iterator_traits<std::iterator_traits<int>>>>();

  test_helpers::generate_failed_requirements_tests<
      iterator_concepts::is_iterator<non_dereferencable_iterator>>();

  test_helpers::generate_failed_requirements_tests<
      iterator_concepts::is_iterator<non_weakly_incrementable_iterator>,
      std::tuple<iterator_concepts::is_weakly_incrementable<
          non_weakly_incrementable_iterator>>>();
}
