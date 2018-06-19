#pragma once

#include <mgs/detail/iterators/adaptive_iterator.hpp>
#include <mgs/detail/meta/concepts/input_transformer.hpp>

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
  {
    _index = 0;
    _encoded = typename EncodingTraits::algorithm{}(_current, _end);
  }
}

template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
auto base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>::
    get() const -> value_type
{
  assert(_index != EncodingTraits::nb_output_bytes);
  return _encoded[_index];
}

template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE>
void base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>::
    seek_forward(difference_type n)
{
  assert(n > 0);
  assert(_index != EncodingTraits::nb_output_bytes);

  while (n-- > 0)
  {
    ++_index;
    if (_index == EncodingTraits::nb_output_bytes)
    {
      if (_current != _end)
      {
        _index = 0;
        _encoded = typename EncodingTraits::algorithm{}(_current, _end);
      }
      else
        assert(n == 0);
    }
  }
}

template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE,
          typename = std::enable_if_t<
              is_input_transformer<base_n_transformer<EncodingTraits,
                                                      UnderlyingIterator,
                                                      Sentinel,
                                                      SFINAE>>::value>>
auto begin(base_n_transformer<EncodingTraits,
                              UnderlyingIterator,
                              Sentinel,
                              SFINAE> const& enc)
{
  return adaptive_iterator<
      base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>,
      std::input_iterator_tag>{enc};
}

template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel,
          typename SFINAE,
          typename = std::enable_if_t<
              is_input_transformer<base_n_transformer<EncodingTraits,
                                                      UnderlyingIterator,
                                                      Sentinel,
                                                      SFINAE>>::value>>
auto end(base_n_transformer<EncodingTraits,
                            UnderlyingIterator,
                            Sentinel,
                            SFINAE> const& enc)
{
  // hack to trick the constructor, avoid encoding values twice
  // no need to set current to end, since _index == 4
  base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>
      hack_enc;
  hack_enc._current = enc._current;
  hack_enc._end = enc._end;
  assert(hack_enc._index == EncodingTraits::nb_output_bytes);
  return adaptive_iterator<
      base_n_transformer<EncodingTraits, UnderlyingIterator, Sentinel, SFINAE>,
      std::input_iterator_tag>{hack_enc};
}

template <typename T, typename U, typename V, typename W>
bool operator==(base_n_transformer<T, U, V, W> const& lhs,
                base_n_transformer<T, U, V, W> const& rhs)
{
  if (lhs._index == EncodingTraits::nb_output_bytes ||
      rhs._index == EncodingTraits::nb_output_bytes)
  {
    return lhs._index == rhs._index;
  }
  return std::tie(lhs._current, lhs._index) ==
         std::tie(rhs._current, rhs._index);
}

template <typename T, typename U, typename V, typename W>
bool operator!=(base_n_transformer<T, U, V, W> const& lhs,
                base_n_transformer<T, U, V, W> const& rhs)
{
  return !(lhs == rhs);
}
}
}
