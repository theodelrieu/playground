#pragma once

#include <tuple>

namespace mgs
{
inline namespace v1
{
// TODO namespace input_adapters?
namespace detail
{
template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
basic_adapter<InputTransformer, UnderlyingIterator, Sentinel, SFINAE>::
    basic_adapter(UnderlyingIterator const& beg, Sentinel const& end)
  : _current(beg), _end(end)
{
  if (_current != _end)
    _process_input();
}

template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
auto basic_adapter<InputTransformer, UnderlyingIterator, Sentinel, SFINAE>::
    get() const -> value_type const&
{
  using std::begin;

  return *(begin(_transformed) + _index);
}

template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
void basic_adapter<InputTransformer, UnderlyingIterator, Sentinel, SFINAE>::
    seek_forward(difference_type n)
{
  assert(n > 0);

  using std::begin;
  using std::end;

  auto const end_it = end(_transformed);

  while (n-- > 0)
  {
    ++_index;
    if ((begin(_transformed) + _index) == end_it)
    {
      if (_current != _end)
        _process_input();
      else
        assert(n == 0);
    }
  }
}

template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
void basic_adapter<InputTransformer, UnderlyingIterator, Sentinel, SFINAE>::
    _process_input()
{
  assert(_current != _end);
  using std::begin;

  _transformed = InputTransformer{}(_current, _end);
  _index = 0;
}

template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
auto basic_adapter<InputTransformer, UnderlyingIterator, Sentinel, SFINAE>::
    begin() const -> iterator
{
  return iterator{*this};
}

template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
auto basic_adapter<InputTransformer, UnderlyingIterator, Sentinel, SFINAE>::end()
    const -> iterator
{
  return iterator{{}};
}

template <typename T, typename U, typename V, typename W>
bool operator==(basic_adapter<T, U, V, W> const& lhs,
                basic_adapter<T, U, V, W> const& rhs)
{
  using std::begin;
  using std::end;

  auto const lhs_transformed_current = begin(lhs._transformed) + lhs._index;
  auto const rhs_transformed_current = begin(rhs._transformed) + rhs._index;
  auto const lhs_transformed_end = end(lhs._transformed);
  auto const rhs_transformed_end = end(rhs._transformed);

  // a bit similar to std::istreambuf_iterator::equal
  if (lhs_transformed_current == lhs_transformed_end ||
      rhs_transformed_current == rhs_transformed_end)
  {
    return lhs_transformed_current == lhs_transformed_end &&
           rhs_transformed_current == rhs_transformed_end;
  }
  return std::tie(lhs._current, lhs._index) ==
         std::tie(rhs._current, rhs._index);
}

template <typename T, typename U, typename V, typename W>
bool operator!=(basic_adapter<T, U, V, W> const& lhs,
                basic_adapter<T, U, V, W> const& rhs)
{
  return !(lhs == rhs);
}
}
}
}
