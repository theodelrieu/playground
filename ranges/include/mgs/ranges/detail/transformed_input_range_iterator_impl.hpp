#pragma once

#include <memory>

namespace mgs
{
namespace detail
{
template <typename TransformedInputRange>
transformed_input_range_iterator<TransformedInputRange>::
    transformed_input_range_iterator(TransformedInputRange const& e)
  : _range(e)
{
  if (_range._index == -1)
    _range._transform_input();
}

template <typename TransformedInputRange>
transformed_input_range_iterator<TransformedInputRange>::
    transformed_input_range_iterator(TransformedInputRange&& e)
  : _range(std::move(e))
{
  if (_range._index == -1)
    _range._transform_input();
}

template <typename TransformedInputRange>
auto transformed_input_range_iterator<TransformedInputRange>::operator*() const
    -> reference
{
  return _range._get();
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
  _range._seek_forward();
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
bool transformed_input_range_iterator<TransformedInputRange>::equals(
    transformed_input_range_iterator<TransformedInputRange> const& rhs) const
{
  return (this->_range._index == -1 && rhs._range._index == -1) ||
         (this->_range._index != -1 && rhs._range._index != -1);
}

template <typename TransformedInputRange>
bool operator==(
    transformed_input_range_iterator<TransformedInputRange> const& lhs,
    transformed_input_range_iterator<TransformedInputRange> const& rhs) noexcept
{
  return lhs.equals(rhs);
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
