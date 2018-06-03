#pragma once

#include <algorithm>
#include <array>
#include <bitset>

#include <mgs/exceptions/parse_error.hpp>

namespace mgs
{
namespace detail
{
struct base64_decode_algorithm
{
  template <typename Iterator, typename Sentinel>
  std::array<std::uint8_t, 3> operator()(Iterator& current, Sentinel const end) const
  {
    static constexpr char const alphabet[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

    auto const alph_begin = std::begin(alphabet);
    auto const alph_end = std::end(alphabet);

    std::array<std::uint8_t, 3> decoded;
    assert(current != end);

    std::bitset<24> bits;
    for (auto i = 0; i < 4; ++i)
    {
      if (current == end)
        throw parse_error{"base64: unexpected EOF"};
      auto c = static_cast<char>(*current++);
      auto const index_it = std::find(alph_begin, alph_end, c);
      if (index_it == alph_end)
        throw parse_error{"base64: invalid character"};
      bits |= (std::distance(alph_begin, index_it) << (18 - (6 * i)));
    }

    for (int j = 0; j < 3; ++j)
    {
      auto shift = (16 - (8 * j));
      auto mask = ((std::bitset<24>(0xFFFFFF) >> 16) << shift);
      auto l = bits & mask;
      l >>= shift;
      auto byte = static_cast<std::uint8_t>(l.to_ulong());
      decoded[j] = byte;
    }
    return decoded;
  }
};
}

namespace decoders
{
template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
base64_lazy_decoder<UnderlyingIterator, Sentinel, SFINAE>::base64_lazy_decoder(
    UnderlyingIterator const& begin, Sentinel const& end)
  : _current(begin), _end(end)
{
  if (_current != _end)
  {
    _index = 0;
    _decoded = detail::base64_decode_algorithm{}(_current, _end);
  }
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_lazy_decoder<UnderlyingIterator, Sentinel, SFINAE>::get() const
    -> value_type
{
  assert(_index != 3);
  return _decoded[_index];
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
void base64_lazy_decoder<UnderlyingIterator, Sentinel, SFINAE>::seek_forward(
    difference_type n)
{
  assert(n > 0);
  assert(_index != 3);

  while (n-- > 0)
  {
    ++_index;
    if (_index == 3)
    {
      if (_current != _end)
      {
        _index = 0;
        _decoded = detail::base64_decode_algorithm{}(_current, _end);
      }
      else
        assert(n == 0);
    }
  }
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_lazy_decoder<UnderlyingIterator, Sentinel, SFINAE>::begin() const
    -> iterator
{
  return {*this};
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_lazy_decoder<UnderlyingIterator, Sentinel, SFINAE>::end() const
    -> iterator
{
  // hack to trick the constructor, avoid encoding values twice
  // no need to set current to end, since _index == 4
  base64_lazy_decoder enc;
  enc._current = _current;
  enc._end = _end;
  assert(enc._index == 3);
  return enc;
}

template <typename T, typename U, typename V>
bool operator==(base64_lazy_decoder<T, U, V> const& lhs,
                base64_lazy_decoder<T, U, V> const& rhs)
{
  if (lhs._index == 3 || rhs._index == 3)
    return lhs._index == rhs._index;
  return std::tie(lhs._current, lhs._index) ==
         std::tie(rhs._current, rhs._index);
}

template <typename T, typename U, typename V>
bool operator!=(base64_lazy_decoder<T, U, V> const& lhs,
                base64_lazy_decoder<T, U, V> const& rhs)
{
  return !(lhs == rhs);
}
}
}
