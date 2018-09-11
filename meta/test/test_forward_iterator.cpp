#include <istream>
#include <iterator>
#include <string>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/iterator/forward_iterator.hpp>
#include <mgs/meta/concepts/iterator/incrementable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include "meta_test_helpers.hpp"

using namespace mgs::meta;
namespace iterator_concepts = concepts::iterator;

namespace
{
struct proxy
{
  int& operator*();
};

struct invalid_post_increment_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  invalid_post_increment_iterator& operator++();
  proxy operator++(int);
  reference operator*();
};

bool operator==(invalid_post_increment_iterator,
                invalid_post_increment_iterator);
bool operator!=(invalid_post_increment_iterator,
                invalid_post_increment_iterator);

struct invalid_reference_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  invalid_reference_iterator& operator++();
  invalid_reference_iterator operator++(int);
  value_type operator*();
};

bool operator==(invalid_reference_iterator, invalid_reference_iterator);
bool operator!=(invalid_reference_iterator, invalid_reference_iterator);

struct valid_forward_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  valid_forward_iterator& operator++();
  valid_forward_iterator operator++(int);
  reference operator*();
};

bool operator==(valid_forward_iterator, valid_forward_iterator);
bool operator!=(valid_forward_iterator, valid_forward_iterator);
}

TEST_CASE("InputIterator", "[meta][concepts][iterator]")
{
  static_assert(iterator_concepts::is_forward_iterator<char*>::value, "");
  static_assert(
      iterator_concepts::is_forward_iterator<valid_forward_iterator>::value, "");

  static_assert(!iterator_concepts::is_forward_iterator<
                    std::istreambuf_iterator<char>>::value,
                "");
  static_assert(!iterator_concepts::is_forward_iterator<
                    invalid_post_increment_iterator>::value,
                "");
  static_assert(!iterator_concepts::is_forward_iterator<
                    invalid_reference_iterator>::value,
                "");

  static_assert(!iterator_concepts::is_forward_iterator<void>::value, "");
  static_assert(!iterator_concepts::is_forward_iterator<struct incomplete>::value, "");

  generate_failed_requirements_tests<
      iterator_concepts::is_forward_iterator<invalid_post_increment_iterator>,
      std::tuple<iterator_concepts::is_incrementable<
          invalid_post_increment_iterator>>>();

  generate_failed_requirements_tests<
      iterator_concepts::is_forward_iterator<std::istreambuf_iterator<char>>>();
}
