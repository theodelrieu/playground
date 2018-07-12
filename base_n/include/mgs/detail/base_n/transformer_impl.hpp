#pragma once

#include <tuple>

namespace mgs
{
// TODO namespace input_transformers?
namespace detail
{
template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>::
    base_n_transformer(UnderlyingIterator const& begin, Sentinel const& end)
  : _current(begin), _end(end)
{
  if (_current != _end)
    _process_input();
}

template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
auto base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>::
    get() const -> value_type
{
  assert(_index != _max_index);
  return _output[_index];
}

template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
void base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>::
    seek_forward(difference_type n)
{
  assert(n > 0);
  assert(_index != _max_index);

  while (n-- > 0)
  {
    ++_index;
    if (_index == _max_index)
    {
      if (_current != _end)
        _process_input();
      else
        assert(n == 0);
    }
  }
}

template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
auto base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>::
    begin() const -> iterator
{
  return iterator{*this};
}

template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
auto base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>::
    end() const -> iterator
{
  return iterator{{}};
}

template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
void base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>::
    _process_input()
{
  auto it = _output.begin();
  typename EncodingTraits::algorithm{}(_current, _end, it);
  _max_index = _output.size() - (_output.end() - it);
  assert(_max_index <= _output.size());
  _index = 0;
}

template <typename T, typename U, typename V, typename W>
bool operator==(base_n_transformer<T, U, V, W> const& lhs,
                base_n_transformer<T, U, V, W> const& rhs)
{
  // a bit similar to std::istreambuf_iterator::equal
  if (lhs._index == lhs._max_index || rhs._index == rhs._max_index)
    return lhs._index == lhs._max_index && rhs._index == rhs._max_index;
  return std::tie(lhs._current, lhs._index, lhs._max_index) ==
         std::tie(rhs._current, rhs._index, rhs._max_index);
}

template <typename T, typename U, typename V, typename W>
bool operator!=(base_n_transformer<T, U, V, W> const& lhs,
                base_n_transformer<T, U, V, W> const& rhs)
{
  return !(lhs == rhs);
}
}
}
