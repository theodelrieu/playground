#pragma once

#include <array>

namespace b64
{
namespace detail
{
struct base64_encode_algorithm
{
  template <typename Iterator, typename Sentinel>
  std::array<char, 4> operator()(Iterator& current, Sentinel const end) const
  {
    static constexpr char const alphabet[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

    std::array<char, 4> encoded;
    assert(current != end);

    std::bitset<24> bits;
    int i = 0;
    for (; i < 3; ++i)
    {
      if (current == end)
        break;
      auto byte = static_cast<std::uint8_t>(*current++);
      bits |= (byte << (16 - (8 * i)));
    }

    for (int j = 0; j < i + 1; ++j)
    {
      auto shift = (18 - (6 * j));
      auto mask = ((std::bitset<24>(0xFFFFFF) >> 18) << shift);
      auto l = bits & mask;
      l >>= shift;
      auto index = static_cast<std::uint8_t>(l.to_ulong());
      encoded[j] = alphabet[index];
    }
    std::fill(std::next(encoded.begin(), i + 1), encoded.end(), '=');
    return encoded;
  }
};
}

namespace encoders
{
template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
    base64_stream_encoder(UnderlyingIterator const& begin, Sentinel const& end)
  : _current(begin), _end(end)
{
  if (_current != _end)
  {
    _index = 0;
    _encoded = detail::base64_encode_algorithm{}(_current, _end);
  }
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::get() const
    -> value_type const&
{
  assert(_index != 4);
  return _encoded[_index];
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::seek_forward(
    difference_type n)
{
  assert(n > 0);
  assert(_index != 4);

  while (n-- > 0)
  {
    ++_index;
    if (_index == 4)
    {
      if (_current != _end)
      {
        _index = 0;
        _encoded = detail::base64_encode_algorithm{}(_current, _end);
      }
      else
        assert(n == 0);
    }
  }
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::begin() const
    -> iterator
{
  return {*this};
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::end() const
    -> iterator
{
  // hack to trick the constructor, avoid encoding values twice
  // no need to set current to end, since _index == 4
  base64_stream_encoder enc;
  enc._current = _current;
  enc._end = _end;
  assert(enc._index == 4);
  return enc;
}

template <typename T, typename U, typename V>
bool operator==(base64_stream_encoder<T, U, V> const& lhs,
                base64_stream_encoder<T, U, V> const& rhs)
{
  if (lhs._index == 4 || rhs._index == 4)
    return lhs._index == rhs._index;
  return std::tie(lhs._current, lhs._index) ==
         std::tie(rhs._current, rhs._index);
}

template <typename T, typename U, typename V>
bool operator!=(base64_stream_encoder<T, U, V> const& lhs,
                base64_stream_encoder<T, U, V> const& rhs)
{
  return !(lhs == rhs);
}
}
}
