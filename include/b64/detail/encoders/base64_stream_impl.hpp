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
  }
};

template <typename CRTP,
          typename IteratorTag =
              iterator_category_t<typename CRTP::underlying_iterator>,
          typename = void>
struct base64_stream_encoder_impl;

template <template <class...> class Encoder,
          typename UnderlyingIterator,
          typename Sentinel>
class base64_stream_encoder_impl<
    Encoder<UnderlyingIterator, Sentinel>,
    std::input_iterator_tag,
    std::enable_if_t<is_input_iterator<UnderlyingIterator>::value &&
                     is_sentinel<Sentinel, UnderlyingIterator>::value &&
                     is_byte_integral<value_type_t<
                         std::iterator_traits<UnderlyingIterator>>>::value>>
{
  using algorithm = base64_encode_algorithm;

protected:
  using iterator = adaptive_iterator<Encoder<UnderlyingIterator, Sentinel>,
                                     std::input_iterator_tag>;
  friend iterator;

  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

  using value_type = char;
  using difference_type = std::streamoff;

  base64_stream_encoder_impl() = default;
  base64_stream_encoder_impl(UnderlyingIterator const& begin,
                             Sentinel const& end)
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
    // input iterators are not multi-pass, no need to store begin, so copying
    // *this is the only thing to do.
    return {*this};
  }

  iterator end() const
  {
    // hack to trick the constructor, avoid encoding values twice
    // _index == 4
    base64_stream_encoder_impl enc;
    enc._current = _current;
    enc._end = _end;
    assert(enc._index == 4);
    return {std::move(enc)};
  }

  // dummy methods
  void seek_backward(difference_type)
  {
    static_assert(sizeof(UnderlyingIterator) == 0,
                  "this function should never be called");
  }

  difference_type pos() const noexcept
  {
    static_assert(sizeof(UnderlyingIterator) == 0,
                  "this function should never be called");
    return 0;
  }

  UnderlyingIterator _current{};
  Sentinel _end{};
  int _index{4};
  std::array<char, 4> _encoded;

  template <typename T, typename U>
  friend bool operator==(
      base64_stream_encoder_impl<T, std::input_iterator_tag, U> const& lhs,
      base64_stream_encoder_impl<T, std::input_iterator_tag, U> const& rhs)
  {
    if (lhs._index == 4 || rhs._index == 4)
      return lhs._index == rhs._index;
    return std::tie(lhs._current, lhs._end) == std::tie(rhs._current, rhs._end);
  }
};

template <typename T, typename U>
bool operator!=(
    base64_stream_encoder_impl<T, std::input_iterator_tag, U> const& lhs,
    base64_stream_encoder_impl<T, std::input_iterator_tag, U> const& rhs)
{
  return !(lhs == rhs);
}
}
}
// {
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
//     base64_stream_encoder(UnderlyingIterator const& begin, Sentinel const& end)
//   : _begin(begin),
//     _current_it(begin),
//     _end(end)
// {
//   if (_current_it != _end)
//   {
//     _last_encoded_index = 0;
//     _encode_next_values();
//   }
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::begin() const
//     -> iterator
// {
//   return _begin_impl(underlying_iterator_tag{});
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::end() const
//     -> iterator
// {
//   // hack to trick the constructor, avoid encoding values twice
//   // _last_encoded_index == 4
//   base64_stream_encoder enc;
//   enc._begin = _begin;
//   enc._current_it = _begin;
//   enc._end = _end;
//   return {std::move(enc)};
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::get() const
//     -> value_type const&
// {
//   return _last_encoded[_last_encoded_index];
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// template <typename, typename>
// auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::pos() const
//     noexcept -> difference_type
// {
//   // this is ensured by RandomAccessIterator
//   static_assert(detail::is_sized_sentinel<UnderlyingIterator, Sentinel>::value,
//                 "");
//
//   auto dist = difference_type{};
//
//   if (_current_it == _end || _last_encoded_index == 4)
//     dist = _end - _begin;
//   else
//   {
//     dist = _current_it - _begin;
//     // FIXME clean this code...
//     if (_current_it != _end && dist > 0)
//       dist -= 3;
//   }
//   auto const res = std::lldiv(dist, 3);
//   return (res.quot + std::min(res.rem, 1ll)) * 4;
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::seek_forward(
//     difference_type n)
// {
//   _seek_forward_impl(n, underlying_iterator_tag{});
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// template <typename, typename>
// void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
//     seek_backward(difference_type n)
// {
//   _seek_backward_impl(n, underlying_iterator_tag{});
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// template <typename, typename>
// auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::_begin_impl(
//     std::input_iterator_tag) const -> iterator
// {
//   return {*this};
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// template <typename, typename>
// auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::_begin_impl(
//     std::forward_iterator_tag) const -> iterator
// {
//   return {{_begin, _end}};
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
//     _encode_next_values()
// {
//   static constexpr char const alphabet[] = {
//       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
//       'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
//       'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
//       'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
//       '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
//
//   assert(_current_it != _end);
//
//   std::bitset<24> bits;
//   int i = 0;
//   for (; i < 3; ++i)
//   {
//     if (_current_it == _end)
//       break;
//     auto byte = static_cast<std::uint8_t>(*_current_it++);
//     bits |= (byte << (16 - (8 * i)));
//   }
//
//   for (int j = 0; j < i + 1; ++j)
//   {
//     auto shift = (18 - (6 * j));
//     auto mask = ((std::bitset<24>(0xFFFFFF) >> 18) << shift);
//     auto l = bits & mask;
//     l >>= shift;
//     auto index = static_cast<std::uint8_t>(l.to_ulong());
//     _last_encoded[j] = alphabet[index];
//   }
//   std::fill(std::next(_last_encoded.begin(), i + 1), _last_encoded.end(), '=');
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// template <typename, typename>
// void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
//     _seek_forward_impl(difference_type n, std::input_iterator_tag)
// {
//   assert(n == 1);
//   assert(_last_encoded_index != 4);
//
//   _last_encoded_index = detail::wrap_integer<0, 3>(_last_encoded_index + 1);
//   if (_last_encoded_index == 0)
//   {
//     if (_current_it == _end)
//       _last_encoded_index = 4;
//     else
//       _encode_next_values();
//   }
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// template <typename, typename>
// void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
//     _seek_forward_impl(difference_type n, std::random_access_iterator_tag)
// {
//   assert(n > 0);
//   assert(_last_encoded_index != 4);
//
//   if (_last_encoded_index + n <= 3)
//   {
//     _last_encoded_index += n;
//     return;
//   }
//   auto const offset = n - (4 - _last_encoded_index);
//   auto const res = std::lldiv(offset, 4);
//   auto const dist = _end - _current_it;
//   _current_it += std::min<std::uint64_t>(dist, res.quot * 3);
//   _last_encoded_index = detail::wrap_integer<0, 3>(_last_encoded_index + n);
//   if (_current_it == _end)
//   {
//     if (res.rem == 0)
//     {
//       _last_encoded_index = 4;
//       return;
//     }
//     _current_it -= dist % 3;
//   }
//   _encode_next_values();
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// template <typename, typename>
// void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
//     _seek_backward_impl(difference_type n, std::bidirectional_iterator_tag)
// {
//   assert(n == -1);
//
//   auto offset = decltype(n)(0);
//   if (_current_it != _end)
//   {
//     assert(_last_encoded_index != 4);
//     _last_encoded_index = detail::wrap_integer<0, 3>(_last_encoded_index - 1);
//     if (_last_encoded_index != 3)
//       return;
//     offset = 3 * 2;
//   }
//   else
//   {
//     offset = std::distance(_begin, _end) % 3;
//     if (offset == 0)
//       offset = 3;
//     // at the very end
//     if (_last_encoded_index == 4)
//       _last_encoded_index = 3;
//     else
//     {
//       _last_encoded_index = detail::wrap_integer<0, 3>(_last_encoded_index - 1);
//       if (_last_encoded_index == 3)
//         offset += 3;
//     }
//   }
//   assert(offset);
//   std::advance(_current_it, -offset);
//   _encode_next_values();
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// template <typename, typename>
// void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
//     _seek_backward_impl(difference_type n, std::random_access_iterator_tag)
// {
//   assert(n < 0);
//
//   if (_last_encoded_index != 4 && _last_encoded_index + n >= 0)
//   {
//     _last_encoded_index += n;
//     return;
//   }
//
//   if (_last_encoded_index == 4)
//     _current_it = _begin + (_end - _begin);
//   auto offset = decltype(n)(0);
//   auto const dist = _current_it - _begin;
//   if (_current_it == _end)
//   {
//     offset = dist % 3;
//     if (offset == 0)
//       offset = 3;
//   }
//   auto res = std::lldiv(n, -4);
//   assert(res.quot >= 0);
//   if (res.quot && res.rem == 0)
//     --res.quot;
//   // TODO refactor this mess
//   if (res.quot == 0 && _last_encoded_index == 0)
//   {
//     if (_current_it == _end)
//       ++res.quot;
//     else
//       res.quot = 2;
//   }
//   offset = std::min<std::uint64_t>(dist, offset + (3 * res.quot));
//   std::advance(_current_it, -offset);
//   _encode_next_values();
//   _last_encoded_index = detail::wrap_integer<0, 3>(n + _last_encoded_index);
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// bool operator==(
//     base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE> const& lhs,
//     base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE> const& rhs)
// {
//   if (lhs._last_encoded_index == 4|| rhs._last_encoded_index == 4)
//     return lhs._last_encoded_index == rhs._last_encoded_index;
//   return std::tie(lhs._begin, lhs._current_it, lhs._last_encoded_index) ==
//          std::tie(rhs._begin, rhs._current_it, rhs._last_encoded_index);
// }
//
// template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
// bool operator!=(
//     base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE> const& lhs,
//     base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE> const& rhs)
// {
//   return !(lhs == rhs);
// }
// }
