#pragma once

#include <b64/detail/meta/concepts/encoding_processor.hpp>

#include <iterator>
#include <ios>
#include <type_traits>

namespace b64
{
template <
    typename EncodingProcessor,
    typename = std::enable_if_t<detail::is_encoding_processor<EncodingProcessor>::value>>
class encoding_iterator
{
public:
  // TODO different value_type depending on EncodingProcess 
  using value_type = char;

  using reference = value_type const&;
  using const_reference = value_type const&;

  using pointer = value_type const*;
  using const_pointer = value_type const*;

  using iterator_category = std::input_iterator_tag;
  using difference_type = std::streamoff;

  encoding_iterator() = default;
  encoding_iterator(EncodingProcessor const&);
  encoding_iterator(encoding_iterator const&) = default;
  encoding_iterator(encoding_iterator&&) = default;
  encoding_iterator& operator=(encoding_iterator const&) =
      default;
  encoding_iterator& operator=(encoding_iterator&&) = default;

  reference operator*() const;
  pointer operator->() const;

  encoding_iterator& operator++();
  encoding_iterator operator++(int);

  template <typename T, typename U>
  friend bool operator==(encoding_iterator<T, U> const&,
                         encoding_iterator<T, U> const&);

private:
  EncodingProcessor _encoding_processor;
  // FIXME put that in EncodingProcessor
  char mutable _last_read{0};
  bool mutable _read{false};
  bool mutable _end{true};
};

template <typename T, typename U>
encoding_iterator<T, U>::encoding_iterator(T const& encoding_processor)
  : _encoding_processor(encoding_processor), _end(false)
{
}

// template <typename T, typename U>
// void encoding_iterator<T, U>::_encode_next_values() const
// {
//   assert(_last_encoded_index == 0);
//
//   std::bitset<24> bits;
//   int i = 0;
//   for (; i < 3; ++i)
//   {
//     if (_current_it == _end)
//       break;
//     auto byte = static_cast<uint8_t>(*_current_it);
//     bits |= (byte << (16 - (8 * i)));
//     ++_current_it;
//   }
//
//   for (int j = 0; j < i + 1; ++j)
//   {
//     auto shift = (18 - (6 * j));
//     auto mask = ((std::bitset<24>(0xFFFFFF) >> 18) << shift);
//     auto l = bits & mask;
//     l >>= shift;
//     auto index = static_cast<uint8_t>(l.to_ulong());
//     _last_encoded[j] = alphabet[index];
//   }
//   std::fill(std::next(_last_encoded.begin(), i + 1), _last_encoded.end(), '=');
// }
//
template <typename T, typename U>
auto encoding_iterator<T, U>::operator*() const -> reference
{
  if (!_read)
  {
    _last_read = _encoding_processor.next_char();
    _read = true;
  }
  return _last_read;
}

template <typename T, typename U>
auto encoding_iterator<T, U>::operator-> () const -> pointer
{
  return std::addressof(**this);
}

template <typename T, typename U>
auto encoding_iterator<T, U>::operator++() -> encoding_iterator&
{
  _read = false;
  _end = _encoding_processor.eof();
  return *this;
}

template <typename T, typename U>
auto encoding_iterator<T, U>::operator++(int) -> encoding_iterator
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename T, typename U>
bool operator==(encoding_iterator<T, U> const& lhs,
                encoding_iterator<T, U> const& rhs)
{
  if (lhs._end || rhs._end)
    return lhs._end == rhs._end;
  return lhs._encoding_processor == rhs._encoding_processor &&
         lhs._last_read == rhs._last_read;
}

template <typename T, typename U>
bool operator!=(encoding_iterator<T, U> const& lhs,
                encoding_iterator<T, U> const& rhs) noexcept(noexcept(lhs ==
                                                                      rhs))
{
  return !(lhs == rhs);
}
}
