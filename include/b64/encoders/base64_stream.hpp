#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <iterator>
#include <tuple>

#include <b64/detail/iterators/adaptive_iterator.hpp>
#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/input_iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>
#include <b64/detail/wrap_integer.hpp>

namespace b64
{
namespace detail
{
template <typename T>
struct is_byte_integral
    : std::integral_constant<bool, std::is_integral<T>::value && sizeof(T) == 1>
{
};
}

namespace encoders
{
template <
    typename UnderlyingIterator,
    typename Sentinel = UnderlyingIterator,
    typename = void>
class base64_stream_encoder;

// TODO move that
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

template <typename UnderlyingIterator, typename Sentinel>
class base64_stream_encoder<
    UnderlyingIterator,
    Sentinel,
    std::enable_if_t<detail::is_input_iterator<UnderlyingIterator>::value &&
                     detail::is_sentinel<Sentinel, UnderlyingIterator>::value &&
                     detail::is_byte_integral<detail::value_type_t<
                         std::iterator_traits<UnderlyingIterator>>>::value>>
{
private:
  using algorithm = base64_encode_algorithm;
  using iterator_category = std::input_iterator_tag;
  using iterator =
      detail::adaptive_iterator<base64_stream_encoder, iterator_category>;

public:
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

  using value_type = char;
  using difference_type = std::streamoff;

  base64_stream_encoder() = default;
  base64_stream_encoder(UnderlyingIterator const& begin, Sentinel const& end)
    : _current(begin), _end(end)
  {
    if (_current != _end)
    {
      _index = 0;
      _encoded = algorithm{}(_current, _end);
    }
  }

  value_type const& get() const
  {
    assert(_index != 4);
    return _encoded[_index];
  }

  void seek_forward(difference_type n)
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
          _encoded = algorithm{}(_current, _end);
        }
        else
          assert(n == 0);
      }
    }
  }

  iterator begin() const
  {
    return {*this};
  }

  iterator end() const
  {
    // hack to trick the constructor, avoid encoding values twice
    // no need to set current to end, since _index == 4
    base64_stream_encoder enc;
    enc._current = _current;
    enc._end = _end;
    assert(enc._index == 4);
    return enc;
  }

private:
  UnderlyingIterator _current{};
  Sentinel _end{};
  int _index{4};
  std::array<char, 4> _encoded;

  template <typename T, typename U, typename V>
  friend bool operator==(base64_stream_encoder<T, U, V> const& lhs,
                         base64_stream_encoder<T, U, V> const& rhs);
};

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

