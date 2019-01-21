#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

#include <mgs/ranges/concepts/transformed_input_range.hpp>
#include <mgs/meta/concepts/core/derived_from.hpp>

namespace mgs
{
inline namespace v1
{
namespace ranges
{
template <typename TransformedInputRange>
class transformed_input_range_iterator
{
  static_assert(ranges::concepts::is_transformed_input_range<
                    TransformedInputRange>::value,
                "template parameter must be a TransformedInputRange");

public:
  using value_type = typename TransformedInputRange::value_type;
  using difference_type = typename TransformedInputRange::difference_type;

  using reference =
      decltype(std::declval<TransformedInputRange const&>().get());
  using const_reference = reference;

  using pointer = value_type const*;
  using const_pointer = pointer;

  using iterator_category = std::input_iterator_tag;

  transformed_input_range_iterator() = default;
  explicit transformed_input_range_iterator(TransformedInputRange const&);

  reference operator*() const;
  pointer operator->() const;

  transformed_input_range_iterator& operator++();
  transformed_input_range_iterator operator++(int);

  template <typename T>
  friend bool operator==(transformed_input_range_iterator<T> const&,
                         transformed_input_range_iterator<T> const&) noexcept;

private:
  TransformedInputRange _range;
};

template <typename TransformedInputRange>
bool operator!=(
    transformed_input_range_iterator<TransformedInputRange> const&,
    transformed_input_range_iterator<
        TransformedInputRange> const&) noexcept;
}
}
}

#include <mgs/ranges/detail/transformed_input_range_iterator_impl.hpp>
