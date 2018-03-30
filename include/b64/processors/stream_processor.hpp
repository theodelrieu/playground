#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>

#include <b64/detail/meta/concepts/input_source.hpp>

namespace b64
{
// TODO ns processors
// TODO fix name
// TODO sfinae InputIterator
template <typename InputSource,
          typename =
              std::enable_if_t<detail::is_input_source<InputSource>::value>>
class stream_processor
{
  static constexpr char const alphabet[] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

public:
  using value_type = typename InputSource::value_type;

  stream_processor() = default;
  stream_processor(InputSource const& source);

  value_type next_char() const;
  bool eof() const;

  template <typename T, typename U>
  friend bool operator==(stream_processor<T, U> const&,
                         stream_processor<T, U> const& rhs);

private:
  void _encode_next_values() const;

  InputSource _source;
  // FIXME should be value_type, char
  std::array<std::uint8_t, 4> mutable _last_encoded;
  int mutable _last_encoded_index{4};
};

template <typename InputSource, typename SFINAE>
char const stream_processor<InputSource, SFINAE>::alphabet[];

template <typename InputSource, typename SFINAE>
stream_processor<InputSource, SFINAE>::stream_processor(
    InputSource const& source)
  : _source(source), _last_encoded_index(0)
{
}

template <typename InputSource, typename SFINAE>
void stream_processor<InputSource, SFINAE>::_encode_next_values() const
{
  assert(_last_encoded_index == 0);

  std::bitset<24> bits;
  int i = 0;
  for (; i < 3; ++i)
  {
    if (_source.eof())
      break;
    // FIXME require 1 byte value_type
    auto byte = static_cast<uint8_t>(_source.next_char());
    bits |= (byte << (16 - (8 * i)));
  }

  for (int j = 0; j < i + 1; ++j)
  {
    auto shift = (18 - (6 * j));
    auto mask = ((std::bitset<24>(0xFFFFFF) >> 18) << shift);
    auto l = bits & mask;
    l >>= shift;
    auto index = static_cast<uint8_t>(l.to_ulong());
    _last_encoded[j] = alphabet[index];
  }
  std::fill(std::next(_last_encoded.begin(), i + 1), _last_encoded.end(), '=');
}

template <typename InputSource, typename SFINAE>
auto stream_processor<InputSource, SFINAE>::next_char() const -> value_type
{
  if (!eof() && _last_encoded_index == 4)
    _last_encoded_index = 0;
  if (_last_encoded_index == 0)
    _encode_next_values();
  return _last_encoded[_last_encoded_index++];
}

template <typename InputSource, typename SFINAE>
bool stream_processor<InputSource, SFINAE>::eof() const
{
  return _last_encoded_index == 4 && _source.eof();
}

template <typename InputSource, typename SFINAE>
bool operator==(stream_processor<InputSource, SFINAE> const& lhs,
                stream_processor<InputSource, SFINAE> const& rhs)
{
  if (lhs._last_encoded_index == 4 &&
      lhs._last_encoded_index == rhs._last_encoded_index)
  {
    return true;
  }
  return lhs._source == rhs._source &&
         lhs._last_encoded_index == rhs._last_encoded_index;
}

template <typename InputSource, typename SFINAE>
bool operator!=(stream_processor<InputSource, SFINAE> const& lhs,
                stream_processor<InputSource, SFINAE> const& rhs)
{
  return !(lhs == rhs);
}
}
