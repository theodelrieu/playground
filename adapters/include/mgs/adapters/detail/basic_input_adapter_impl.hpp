#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <tuple>

#include <mgs/adapters/basic_input_adapter.hpp>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
template <typename Impl>
basic_input_adapter<Impl>::basic_input_adapter(underlying_iterator begin,
                                               underlying_sentinel end)
  : Impl(std::move(begin), std::move(end))
{
  _process_input();
}

template <typename Impl>
auto basic_input_adapter<Impl>::get() const -> value_type const&
{
  using std::begin;

  return *(begin(_buffer) + _index);
}

template <typename Impl>
template <typename OutputIterator>
std::size_t basic_input_adapter<Impl>::write(OutputIterator out, std::size_t n)
{
  std::size_t nb_read{};

  while (n > 0 && _buffer.size() != 0)
  {
    using std::end;

    auto const to_read = (std::min)(n, _buffer.size() - _index);
    std::copy_n(end(_buffer) - to_read, to_read, out);
    _index += to_read;
    nb_read += to_read;
    n -= nb_read;
    if (_index == _buffer.size())
      _process_input();
  }
  return nb_read;
}

template <typename Impl>
template <typename OutputIterator>
std::size_t basic_input_adapter<Impl>::write(OutputIterator out)

{
  std::size_t nb_read{};

  while (_buffer.size() != 0)
  {
    using std::end;

    auto const to_read = _buffer.size() - _index;
    std::copy_n(end(_buffer) - to_read, to_read, out);
    nb_read += to_read;
    _process_input();
  }
  return nb_read;
}

template <typename Impl>
void basic_input_adapter<Impl>::seek_forward(difference_type n)
{
  assert(n > 0);

  using std::begin;
  using std::end;

  auto const end_it = end(_buffer);

  while (n-- > 0)
  {
    ++_index;
    if ((begin(_buffer) + _index) == end_it)
      _process_input();
  }
}

template <typename Impl>
void basic_input_adapter<Impl>::_process_input()
{
  using std::begin;

  static_cast<Impl&> (*this)(_buffer);
  _index = 0;
}

template <typename Impl>
auto basic_input_adapter<Impl>::begin() const -> iterator
{
  return iterator{*this};
}

template <typename Impl>
auto basic_input_adapter<Impl>::end() const -> iterator
{
  return iterator{{}};
}

template <typename T>
bool operator==(basic_input_adapter<T> const& lhs,
                basic_input_adapter<T> const& rhs)
{
  using std::begin;
  using std::end;

  auto const lhs_buffer_current = begin(lhs._buffer) + lhs._index;
  auto const rhs_buffer_current = begin(rhs._buffer) + rhs._index;
  auto const lhs_buffer_end = end(lhs._buffer);
  auto const rhs_buffer_end = end(rhs._buffer);

  // a bit similar to std::istreambuf_iterator::equal
  if (lhs_buffer_current == lhs_buffer_end ||
      rhs_buffer_current == rhs_buffer_end)
  {
    return lhs_buffer_current == lhs_buffer_end &&
           rhs_buffer_current == rhs_buffer_end;
  }
  // FIXME compare Impl as well
  return lhs._index == rhs._index;
}

template <typename T>
bool operator!=(basic_input_adapter<T> const& lhs,
                basic_input_adapter<T> const& rhs)
{
  return !(lhs == rhs);
}
}
}
}
