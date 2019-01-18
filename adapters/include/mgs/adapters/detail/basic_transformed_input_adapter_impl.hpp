#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <tuple>

#include <mgs/adapters/basic_transformed_input_adapter.hpp>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
template <typename InputTransformer>
basic_transformed_input_adapter<InputTransformer>::
    basic_transformed_input_adapter(underlying_iterator begin,
                                    underlying_sentinel end)
  : InputTransformer(std::move(begin), std::move(end))
{
  _transform_input();
}

template <typename InputTransformer>
auto basic_transformed_input_adapter<InputTransformer>::get() const
    -> value_type const&
{
  using std::begin;

  return *(begin(_buffer) + _index);
}

template <typename InputTransformer>
template <typename OutputIterator>
std::size_t basic_transformed_input_adapter<InputTransformer>::read(
    OutputIterator out, std::size_t n)
{
  std::size_t nb_read{};

  while (n > 0 && _buffer_size() != 0)
  {
    using std::begin;

    auto const to_read = (std::min)(n, _buffer_size() - _index);
    out = std::copy_n(begin(_buffer) + _index, to_read, out);
    _index += to_read;
    nb_read += to_read;
    n -= nb_read;
    if (_index == _buffer_size())
      _transform_input();
  }
  return nb_read;
}

template <typename InputTransformer>
void basic_transformed_input_adapter<InputTransformer>::seek_forward(
    difference_type n)
{
  assert(n > 0);

  using std::begin;
  using std::end;

  auto const end_it = end(_buffer);

  while (n-- > 0)
  {
    ++_index;
    if ((begin(_buffer) + _index) == end_it)
      _transform_input();
  }
}

template <typename InputTransformer>
void basic_transformed_input_adapter<InputTransformer>::_transform_input()
{
  using std::begin;

  static_cast<InputTransformer&> (*this)(_buffer);
  _index = 0;
}

template <typename InputTransformer>
std::size_t basic_transformed_input_adapter<InputTransformer>::_buffer_size() const
{
  using std::begin;
  using std::end;

  return end(_buffer) - begin(_buffer);
}

template <typename InputTransformer>
template <typename T, typename SFINAE>
std::size_t
basic_transformed_input_adapter<InputTransformer>::max_transformed_size() const
{
  return _buffer_size() - _index +
         static_cast<InputTransformer const&>(*this).max_transformed_size();
}

template <typename InputTransformer>
auto basic_transformed_input_adapter<InputTransformer>::begin() const
    -> iterator
{
  return iterator{*this};
}

template <typename InputTransformer>
auto basic_transformed_input_adapter<InputTransformer>::end() const -> iterator
{
  return iterator{{}};
}

template <typename T>
bool operator==(basic_transformed_input_adapter<T> const& lhs,
                basic_transformed_input_adapter<T> const& rhs)
{
  using std::begin;
  using std::end;

  auto const lhs_buffer_current = begin(lhs._buffer) + lhs._index;
  auto const rhs_buffer_current = begin(rhs._buffer) + rhs._index;
  auto const lhs_buffer_end = end(lhs._buffer);
  auto const rhs_buffer_end = end(rhs._buffer);

  if (lhs_buffer_current == lhs_buffer_end ||
      rhs_buffer_current == rhs_buffer_end)
  {
    return lhs_buffer_current == lhs_buffer_end &&
           rhs_buffer_current == rhs_buffer_end;
  }
  return lhs._index == rhs._index &&
         static_cast<T const&>(lhs) == static_cast<T const&>(rhs);
}

template <typename T>
bool operator!=(basic_transformed_input_adapter<T> const& lhs,
                basic_transformed_input_adapter<T> const& rhs)
{
  return !(lhs == rhs);
}
}
}
}
