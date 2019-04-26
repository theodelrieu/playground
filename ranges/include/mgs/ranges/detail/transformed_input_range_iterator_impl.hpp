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
}

template <typename TransformedInputRange>
transformed_input_range_iterator<TransformedInputRange>::
    transformed_input_range_iterator(TransformedInputRange&& e)
  : _range(std::move(e))
{
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
  using std::begin;
  using std::end;

  auto const lhs_buffer_size = this->_range._buffer_size();
  auto const rhs_buffer_size = rhs._range._buffer_size();

  auto const& lhs_current = this->_range.get_iterator();
  auto const& rhs_current = rhs._range.get_iterator();

  auto const& lhs_end = this->_range.get_sentinel();
  auto const& rhs_end = rhs._range.get_sentinel();
  return lhs_buffer_size == rhs_buffer_size &&
         this->_range._index == rhs._range._index &&
         ((lhs_current == lhs_end || rhs_current == rhs_end) ?
              (lhs_current == lhs_end && rhs_current == rhs_end) :
              lhs_current == rhs_current);
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
