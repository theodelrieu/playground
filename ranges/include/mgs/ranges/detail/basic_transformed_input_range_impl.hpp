#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <tuple>
#include <utility>

#include <mgs/ranges/basic_transformed_input_range.hpp>

namespace mgs
{
namespace ranges
{
template <typename InputTransformer>
basic_transformed_input_range<InputTransformer>::basic_transformed_input_range(
    underlying_iterator begin, underlying_sentinel end)
  : InputTransformer(std::move(begin), std::move(end))
{
  _transform_input();
}

template <typename InputTransformer>
auto basic_transformed_input_range<InputTransformer>::_get() const
    -> value_type const&
{
  using std::begin;

  return *(begin(_buffer) + _index);
}

template <typename InputTransformer>
template <typename OutputIterator, typename>
meta::ssize_t basic_transformed_input_range<InputTransformer>::read(
    OutputIterator out, meta::ssize_t n)
{
  meta::ssize_t nb_read{};

  while (n > 0 && _buffer_size() != 0)
  {
    using std::begin;

    auto const to_read = (std::min)(n, _buffer_size() - _index);
    out = std::copy_n(begin(_buffer) + _index, to_read, out);
    _index += to_read;
    nb_read += to_read;
    n -= to_read;
    if (_index == _buffer_size())
      _transform_input();
  }
  return nb_read;
}

template <typename InputTransformer>
void basic_transformed_input_range<InputTransformer>::_seek_forward()
{
  using std::begin;
  using std::end;

  auto const end_it = end(_buffer);

  ++_index;
  if ((begin(_buffer) + _index) == end_it)
    _transform_input();
}

template <typename InputTransformer>
void basic_transformed_input_range<InputTransformer>::_transform_input()
{
  using std::begin;

  static_cast<InputTransformer&> (*this)(_buffer);
  _index = 0;
}

template <typename InputTransformer>
meta::ssize_t basic_transformed_input_range<InputTransformer>::_buffer_size()
    const
{
  using std::begin;
  using std::end;

  return end(_buffer) - begin(_buffer);
}

template <typename InputTransformer>
template <typename T, typename SFINAE>
meta::ssize_t
basic_transformed_input_range<InputTransformer>::max_transformed_size() const
{
  auto const max_transformed_size = static_cast<InputTransformer const&>(*this).max_transformed_size();
  if (max_transformed_size == -1)
    return -1;
  return _buffer_size() - _index + max_transformed_size;
}

template <typename InputTransformer>
auto basic_transformed_input_range<InputTransformer>::begin() const -> iterator
{
  return iterator{*this};
}

template <typename InputTransformer>
auto basic_transformed_input_range<InputTransformer>::end() const -> iterator
{
  return iterator{{}};
}
}
}
