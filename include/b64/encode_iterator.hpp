#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <ios>
#include <iterator>

namespace b64
{
constexpr char const alphabet[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

template <typename InputIterator, typename Sentinel = InputIterator>
class base64_encoding_iterator
{
public:

  using value_type = char;

  using reference = value_type const&;
  using const_reference = value_type const&;

  using pointer = value_type const*;
  using const_pointer = value_type const*;

  using iterator_category = std::input_iterator_tag;
  using difference_type = std::streamoff;

  base64_encoding_iterator() = default;
  base64_encoding_iterator(InputIterator begin, Sentinel end);
  base64_encoding_iterator(base64_encoding_iterator const&) = default;
  base64_encoding_iterator(base64_encoding_iterator&&) = default;
  base64_encoding_iterator& operator=(base64_encoding_iterator const&) =
      default;
  base64_encoding_iterator& operator=(base64_encoding_iterator&&) = default;

  reference operator*() const;
  pointer operator->() const;

  base64_encoding_iterator& operator++();
  base64_encoding_iterator operator++(int);

  template <typename T, typename U>
  friend bool operator==(base64_encoding_iterator<T, U>,
                         base64_encoding_iterator<T, U>) noexcept;

private:
  void _encode_next_values() const;

  InputIterator mutable _current_it;
  Sentinel _end;
  std::array<value_type, 4> mutable _last_encoded;
  int mutable _last_encoded_index{4};
};

template <typename T, typename U>
base64_encoding_iterator<T, U>::base64_encoding_iterator(T begin, U end)
  : _current_it(begin), _end(end), _last_encoded_index{0}
{
}

template <typename T, typename U>
void base64_encoding_iterator<T, U>::_encode_next_values() const
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

template <typename T, typename U>
auto base64_encoding_iterator<T, U>::operator*() const -> reference
{
  if (_last_encoded_index == 0)
    _encode_next_values();
  return _last_encoded[_last_encoded_index];
}

template <typename T, typename U>
auto base64_encoding_iterator<T, U>::operator->() const -> pointer
{
  return std::addressof(**this);
}

template <typename T, typename U>
auto base64_encoding_iterator<T, U>::operator++() -> base64_encoding_iterator&
{
  assert(_last_encoded_index != 4);
  ++_last_encoded_index;
  if (_current_it != _end && _last_encoded_index == 4)
    _last_encoded_index = 0;
  return *this;
}

template <typename T, typename U>
auto base64_encoding_iterator<T, U>::operator++(int) -> base64_encoding_iterator
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename T, typename U>
bool operator==(base64_encoding_iterator<T, U> lhs,
                base64_encoding_iterator<T, U> rhs) noexcept
{
  // end value
  if (lhs._last_encoded_index == 4 && lhs._last_encoded_index == rhs._last_encoded_index)
    return true;
  return lhs._current_it == rhs._current_it && lhs._end == rhs._end &&
         lhs._last_encoded_index == rhs._last_encoded_index;
}

template <typename T, typename U>
bool operator!=(base64_encoding_iterator<T, U> lhs,
                base64_encoding_iterator<T, U> rhs) noexcept
{
  return !(lhs == rhs);
}

}
