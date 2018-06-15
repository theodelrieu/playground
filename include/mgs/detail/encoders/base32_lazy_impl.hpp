#pragma once

#include <array>
#include <bitset>

namespace mgs
{
namespace detail
{
struct base32_encode_algorithm
{
  template <typename Iterator, typename Sentinel>
  std::array<char, 8> operator()(Iterator& current, Sentinel const end) const
  {
    static constexpr char const alphabet[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '2', '3', '4', '5', '6', '7'};

    std::array<char, 8> encoded;
    assert(current != end);

    std::bitset<40> bits;
    int i = 0;
    for (; i < 5; ++i)
    {
      if (current == end)
        break;
      auto byte = static_cast<std::uint64_t>(*current++);
      bits |= (byte << (32 - (8 * i)));
    }

    for (int j = 0; j < i + 1; ++j)
    {
      auto shift = (35 - (5 * j));
      auto mask = ((std::bitset<40>(0xFFFFFFFFFF) >> 35) << shift);
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
base32_lazy_encoder<UnderlyingIterator, Sentinel, SFINAE>::base32_lazy_encoder(
    UnderlyingIterator const& begin, Sentinel const& end)
  : _current(begin), _end(end)
{
  if (_current != _end)
  {
    _index = 0;
    _encoded = detail::base32_encode_algorithm{}(_current, _end);
  }
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base32_lazy_encoder<UnderlyingIterator, Sentinel, SFINAE>::get() const
    -> value_type
{
  assert(_index != 8);
  return _encoded[_index];
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
void base32_lazy_encoder<UnderlyingIterator, Sentinel, SFINAE>::seek_forward(
    difference_type n)
{
  assert(n > 0);
  assert(_index != 8);

  while (n-- > 0)
  {
    ++_index;
    if (_index == 8)
    {
      if (_current != _end)
      {
        _index = 0;
        _encoded = detail::base32_encode_algorithm{}(_current, _end);
      }
      else
        assert(n == 0);
    }
  }
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base32_lazy_encoder<UnderlyingIterator, Sentinel, SFINAE>::begin() const
    -> iterator
{
  return {*this};
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base32_lazy_encoder<UnderlyingIterator, Sentinel, SFINAE>::end() const
    -> iterator
{
  // hack to trick the constructor, avoid encoding values twice
  // no need to set current to end, since _index == 8
  base32_lazy_encoder enc;
  enc._current = _current;
  enc._end = _end;
  assert(enc._index == 8);
  return enc;
}

template <typename T, typename U, typename V>
bool operator==(base32_lazy_encoder<T, U, V> const& lhs,
                base32_lazy_encoder<T, U, V> const& rhs)
{
  if (lhs._index == 8 || rhs._index == 8)
    return lhs._index == rhs._index;
  return std::tie(lhs._current, lhs._index) ==
         std::tie(rhs._current, rhs._index);
}

template <typename T, typename U, typename V>
bool operator!=(base32_lazy_encoder<T, U, V> const& lhs,
                base32_lazy_encoder<T, U, V> const& rhs)
{
  return !(lhs == rhs);
}
}
}
