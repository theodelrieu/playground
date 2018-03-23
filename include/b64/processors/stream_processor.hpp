#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>

namespace b64
{
// TODO ns processors
// TODO fix name
// TODO sfinae InputIterator
template <typename InputIterator, typename Sentinel = InputIterator>
class stream_processor
{
  static constexpr char const alphabet[] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

public:
  stream_processor() = default;
  stream_processor(InputIterator begin, Sentinel end);
  stream_processor(stream_processor const&) = default;
  stream_processor(stream_processor&&) = default;
  ~stream_processor() = default;

  stream_processor& operator=(stream_processor const&) = default;
  stream_processor& operator=(stream_processor&&) = default;

  std::uint8_t next_char() const;
  bool eof() const;

  template <typename T, typename U>
  friend bool operator==(
      stream_processor<T, U> const&,
      stream_processor<T, U> const& rhs) noexcept(noexcept(std::declval<T>() ==
                                                           std::declval<T>()) &&
                                                  noexcept(std::declval<U>() ==
                                                           std::declval<U>()));

private:
  void _encode_next_values() const;

  InputIterator mutable _current_it;
  Sentinel _end;
  // FIXME should be value_type, char
  std::array<std::uint8_t, 4> mutable _last_encoded;
  int mutable _last_encoded_index{4};
};

template <typename InputIterator, typename Sentinel>
char const stream_processor<InputIterator, Sentinel>::alphabet[];

template <typename InputIterator, typename Sentinel>
stream_processor<InputIterator, Sentinel>::stream_processor(InputIterator begin,
                                                            Sentinel end)
  : _current_it(begin), _end(end), _last_encoded_index(0)
{
}

template <typename InputIterator, typename Sentinel>
void stream_processor<InputIterator, Sentinel>::_encode_next_values() const
{
  assert(_last_encoded_index == 0);

  std::bitset<24> bits;
  int i = 0;
  for (; i < 3; ++i)
  {
    if (_current_it == _end)
      break;
    auto byte = static_cast<uint8_t>(*_current_it);
    bits |= (byte << (16 - (8 * i)));
    ++_current_it;
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

template <typename InputIterator, typename Sentinel>
std::uint8_t stream_processor<InputIterator, Sentinel>::next_char() const
{
  if (!eof() && _last_encoded_index == 4)
    _last_encoded_index = 0;
  if (_last_encoded_index == 0)
    _encode_next_values();
  return _last_encoded[_last_encoded_index++];
}

template <typename InputIterator, typename Sentinel>
bool stream_processor<InputIterator, Sentinel>::eof() const
{
  return _current_it == _end && _last_encoded_index == 4;
}

template <typename InputIterator, typename Sentinel>
bool operator==(stream_processor<InputIterator, Sentinel> const& lhs,
                stream_processor<InputIterator, Sentinel> const&
                    rhs) noexcept(noexcept(std::declval<InputIterator>() ==
                                           std::declval<InputIterator>()) &&
                                  noexcept(std::declval<Sentinel>() ==
                                           std::declval<Sentinel>()))
{
  if (lhs._last_encoded_index == 4 &&
      lhs._last_encoded_index == rhs._last_encoded_index)
  {
    return true;
  }
  return lhs._current_it == rhs._current_it && lhs._end == rhs._end &&
         lhs._last_encoded_index == rhs._last_encoded_index;
}
}
