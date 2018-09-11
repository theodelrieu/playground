#include <iterator>
#include <list>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/iterator/random_access_iterator.hpp>
#include <mgs/meta/static_asserts.hpp>

#include "meta_test_helpers.hpp"

using namespace mgs::meta;
namespace iterator_concepts = concepts::iterator;

namespace
{
struct valid_random_access_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  valid_random_access_iterator& operator++();
  valid_random_access_iterator operator++(int);
  valid_random_access_iterator& operator--();
  valid_random_access_iterator operator--(int);

  valid_random_access_iterator& operator+=(difference_type);
  valid_random_access_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  valid_random_access_iterator operator+(difference_type) const;
  valid_random_access_iterator operator-(difference_type) const;
  friend valid_random_access_iterator operator+(difference_type, valid_random_access_iterator);
  friend difference_type operator-(valid_random_access_iterator,
                                   valid_random_access_iterator);
  reference operator*();
};

bool operator==(valid_random_access_iterator, valid_random_access_iterator);
bool operator!=(valid_random_access_iterator, valid_random_access_iterator);
bool operator<(valid_random_access_iterator, valid_random_access_iterator);
bool operator<=(valid_random_access_iterator, valid_random_access_iterator);
bool operator>(valid_random_access_iterator, valid_random_access_iterator);
bool operator>=(valid_random_access_iterator, valid_random_access_iterator);
}

auto _ = trigger_static_asserts<iterator_concepts::is_random_access_iterator<valid_random_access_iterator>>();

TEST_CASE("RandomAccessIterator", "[meta][concepts][iterator]")
{
  static_assert(iterator_concepts::is_random_access_iterator<char*>::value, "");
  static_assert(iterator_concepts::is_random_access_iterator<
                    valid_random_access_iterator>::value,
                "");

  static_assert(!iterator_concepts::is_random_access_iterator<
                    std::list<char>>::value,
                "");
  static_assert(!iterator_concepts::is_random_access_iterator<void>::value, "");
  static_assert(!iterator_concepts::is_random_access_iterator<struct incomplete>::value, "");
}
