#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>

#include <optional.hpp>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/wrap_integer.hpp>

namespace b64
{
namespace detail
{
template <typename Iterator, typename Sentinel>
std::array<char, 4> encode_base64_values(Iterator& current, Sentinel const& end)
{
  static constexpr char const alphabet[] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

  std::array<char, 4> last_encoded;
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
    last_encoded[j] = alphabet[index];
  }
  std::fill(std::next(last_encoded.begin(), i + 1), last_encoded.end(), '=');
  return last_encoded;
}

template <typename Iterator, typename Sentinel>
void seek_forward(Iterator const& begin,
                  Iterator& current,
                  Sentinel const& end,
                  difference_type_t<std::iterator_traits<Iterator>> n,
                  nonstd::optional<std::array<char, 4>>& encoded,
                  nonstd::optional<wrap_integer<0, 3>>& index,
                  std::input_iterator_tag)
{
  assert(n == 1);
  assert(index);

  if (++*index == 0)
  {
    if (current == end)
      index = nonstd::nullopt;
    else
      encoded = encode_base64_values(current, end);
  }
}

template <typename Iterator, typename Sentinel>
void seek_forward(Iterator const& begin,
                  Iterator& current,
                  Sentinel const& end,
                  difference_type_t<std::iterator_traits<Iterator>> n,
                  nonstd::optional<std::array<char, 4>>& encoded,
                  nonstd::optional<wrap_integer<0, 3>>& index,
                  std::random_access_iterator_tag)
{
  assert(n > 0);
  assert(index);

  if (*index + n <= 3)
  {
    *index += n;
    return;
  }
  auto const offset = n - (4 - *index);
  auto const res = std::lldiv(offset, 4);
  auto const dist = end - current;
  *index += n;
  current += std::min<std::uint64_t>(dist, res.quot * 3);
  if (current == end)
  {
    if (res.rem == 0)
    {
      index = nonstd::nullopt;
      return;
    }
    current -= dist % 3;
  }
  encoded = encode_base64_values(current, end);
}

template <typename Iterator, typename Sentinel>
void seek_backward(Iterator const& begin,
                   Iterator& current,
                   Sentinel const& end,
                   difference_type_t<std::iterator_traits<Iterator>> n,
                   nonstd::optional<std::array<char, 4>>& encoded,
                   nonstd::optional<wrap_integer<0, 3>>& index,
                   std::bidirectional_iterator_tag)
{
  assert(n == -1);

  auto offset = decltype(n)(0);
  if (current != end)
  {
    assert(index);
    assert(encoded);
    if (--*index != 3)
      return;
    offset = 3 * 2;
  }
  else
  {
    offset = std::distance(begin, end) % 3;
    if (offset == 0)
      offset = 3;
    if (!index)
      index.emplace(3);
    else if (--*index == 3)
      offset += 3;
  }
  assert(offset);
  std::advance(current, -offset);
  encoded = encode_base64_values(current, end);
}

template <typename Iterator, typename Sentinel>
void seek_backward(Iterator const& begin,
                   Iterator& current,
                   Sentinel const& end,
                   difference_type_t<Iterator> n,
                   nonstd::optional<std::array<char, 4>>& encoded,
                   nonstd::optional<wrap_integer<0, 3>>& index,
                   std::random_access_iterator_tag)
{
  // FIXME lol
  while (n < 0)
  {
    seek_backward(begin,
                  current,
                  end,
                  -1,
                  encoded,
                  index,
                  std::bidirectional_iterator_tag{});
    ++n;
  }
  // assert(n < 0);
  // auto const idx = (_last_encoded_index ? *_last_encoded_index + n : 4 + n);
  // _last_encoded_index.emplace(idx);
  // if (idx >= 0)
  //   return;
  //
  // auto res = std::ldiv(idx, 4);
  // if (_current_it == _end)
  // {
  //   // we have to reset the underlying iterator so that
  //   // distance(begin, current) % 3 == 0
  //   auto const nb_read =
  //       3 - std::count(_last_encoded->begin() + 2, _last_encoded->end(), '=');
  //   std::advance(_current_it, -nb_read);
  // }
  // else
  //   res.quot--;
  // if (res.rem)
  //   res.quot--;
  // std::advance(_current_it, res.quot * 3);
  // _encode_next_values();
}
}

namespace encoders
{
template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
    base64_stream_encoder(UnderlyingIterator const& begin, Sentinel const& end)
  : _begin(begin),
    _current_it(begin),
    _end(end)
{
  if (_current_it != _end)
  {
    _last_encoded_index.emplace(0);
    _last_encoded = detail::encode_base64_values(_current_it, _end);
  }
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::begin() const
    -> iterator
{
  // FIXME
  return {*this};
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::end() const
    -> iterator
{
  // hack to trick the constructor, avoid encoding values twice
  // _last_encoded_index == nullopt
  base64_stream_encoder enc;
  enc._begin = _begin;
  enc._current_it = _begin;
  enc._end = _end;
  return {std::move(enc)};
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::get() const
    -> value_type const&
{
  return _last_encoded->operator[](*_last_encoded_index);
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
template <typename, typename>
auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::pos() const
    noexcept -> difference_type
{
  // this is ensured by RandomAccessIterator
  static_assert(detail::is_sized_sentinel<UnderlyingIterator, Sentinel>::value,
                "");

  auto dist = difference_type{};

  if (_current_it == _end || !_last_encoded_index)
    dist = _end - _begin;
  else
  {
    dist = _current_it - _begin;
    // FIXME clean this code...
    if (_current_it != _end && dist > 0)
      dist -= 3;
  }
  auto const res = std::lldiv(dist, 3);
  return (res.quot + std::min(res.rem, 1ll)) * 4;
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::seek_forward(
    difference_type n)
{
  using tag =
      detail::iterator_category_t<std::iterator_traits<UnderlyingIterator>>;

  detail::seek_forward(
      _begin, _current_it, _end, n, _last_encoded, _last_encoded_index, tag{});
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
template <typename, typename>
void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
    seek_backward(difference_type n)
{
  using tag =
      detail::iterator_category_t<std::iterator_traits<UnderlyingIterator>>;

  detail::seek_backward(
      _begin, _current_it, _end, n, _last_encoded, _last_encoded_index, tag{});
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
bool operator==(
    base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE> const& lhs,
    base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE> const& rhs)
{
  if (!lhs._last_encoded_index || !rhs._last_encoded_index)
    return lhs._last_encoded_index == rhs._last_encoded_index;
  return std::tie(lhs._begin, lhs._current_it, lhs._last_encoded_index) ==
         std::tie(rhs._begin, rhs._current_it, rhs._last_encoded_index);
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
bool operator!=(
    base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE> const& lhs,
    base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE> const& rhs)
{
  return !(lhs == rhs);
}
}
}
