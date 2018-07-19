#pragma once

#include <tuple>

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel>
transformer_adapter<InputTransformer, UnderlyingIterator, Sentinel>::
    transformer_adapter(UnderlyingIterator const& beg, Sentinel const& end)
  : _current(beg), _end(end)
{
  if (_current != _end)
    _process_input();
}

template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel>
auto transformer_adapter<InputTransformer, UnderlyingIterator, Sentinel>::get()
    const -> value_type const&
{
  using std::begin;

  return *(begin(_transformed) + _index);
}

template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel>
void transformer_adapter<InputTransformer, UnderlyingIterator, Sentinel>::
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
          typename Sentinel>
void transformer_adapter<InputTransformer, UnderlyingIterator, Sentinel>::
    _process_input()
{
  assert(_current != _end);
  using std::begin;

  _transformed = InputTransformer{}(_current, _end);
  _index = 0;
}

template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel>
auto transformer_adapter<InputTransformer, UnderlyingIterator, Sentinel>::
    begin() const -> iterator
{
  return iterator{*this};
}

template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel>
auto transformer_adapter<InputTransformer, UnderlyingIterator, Sentinel>::end()
    const -> iterator
{
  return iterator{{}};
}

template <typename T, typename U, typename V>
bool operator==(transformer_adapter<T, U, V> const& lhs,
                transformer_adapter<T, U, V> const& rhs)
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

template <typename T, typename U, typename V>
bool operator!=(transformer_adapter<T, U, V> const& lhs,
                transformer_adapter<T, U, V> const& rhs)
{
  return !(lhs == rhs);
}
}
}
}
