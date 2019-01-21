#pragma once

#include <memory>

namespace mgs
{
inline namespace v1
{
namespace ranges
{
namespace detail
{
template <typename TransformedInputRange>
transformed_input_range_iterator<TransformedInputRange>::
    transformed_input_range_iterator(TransformedInputRange const& e)
  : _range(e)
{
}

template <typename TransformedInputRange>
auto transformed_input_range_iterator<TransformedInputRange>::operator*() const
    -> reference
{
  return _range.get();
}

template <typename TransformedInputRange>
auto transformed_input_range_iterator<TransformedInputRange>::operator-> ()
    const -> pointer
{
  return std::addressof(**this);
}

template <typename TransformedInputRange>
auto transformed_input_range_iterator<TransformedInputRange>::operator++()
    -> transformed_input_range_iterator<TransformedInputRange>&
{
  _range.seek_forward(1);
  return *this;
}

template <typename TransformedInputRange>
auto transformed_input_range_iterator<TransformedInputRange>::operator++(int)
    -> transformed_input_range_iterator<TransformedInputRange>
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename TransformedInputRange>
bool operator==(
    transformed_input_range_iterator<TransformedInputRange> const& lhs,
    transformed_input_range_iterator<TransformedInputRange> const& rhs) noexcept
{
  return lhs._range == rhs._range;
}

template <typename TransformedInputRange>
bool operator!=(
    transformed_input_range_iterator<TransformedInputRange> const& lhs,
    transformed_input_range_iterator<TransformedInputRange> const& rhs) noexcept
{
  return !(lhs == rhs);
}
}
}
}
}
