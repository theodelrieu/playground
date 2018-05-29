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
                  int& index,
                  std::input_iterator_tag)
{
  assert(n == 1);
  assert(index != 4);

  index = wrap_integer<0, 3>(index + 1);
  if (index == 0)
  {
    if (current == end)
      index = 4;
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
                  int& index,
                  std::random_access_iterator_tag)
{
  assert(n > 0);
  assert(index != 4);

  if (index + n <= 3)
  {
    index += n;
    return;
  }
  auto const offset = n - (4 - index);
  auto const res = std::lldiv(offset, 4);
  auto const dist = end - current;
  current += std::min<std::uint64_t>(dist, res.quot * 3);
  index = wrap_integer<0, 3>(index + n);
  if (current == end)
  {
    if (res.rem == 0)
    {
      index = 4;
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
                   int& index, 
                   std::bidirectional_iterator_tag)
{
  assert(n == -1);

  auto offset = decltype(n)(0);
  if (current != end)
  {
    assert(index != 4);
    assert(encoded);
    index = wrap_integer<0, 3>(index - 1);
    if (index != 3)
      return;
    offset = 3 * 2;
  }
  else
  {
    offset = std::distance(begin, end) % 3;
    if (offset == 0)
      offset = 3;
    // at the very end
    if (index == 4)
      index = 3;
    else
    {
      index = wrap_integer<0, 3>(index - 1);
      if (index == 3)
        offset += 3;
    }
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
                   int& index,
                   std::random_access_iterator_tag)
{
  assert(n < 0);

  if (index != 4 && index + n >= 0)
  {
    index += n;
    return;
  }

  if (index == 4)
    current = begin + (end - begin);
  auto offset = decltype(n)(0);
  auto const dist = current - begin;
  if (current == end)
  {
    offset = dist % 3;
    if (offset == 0)
      offset = 3;
  }
  auto res = std::lldiv(n, -4);
  assert(res.quot >= 0);
  if (res.quot && res.rem == 0)
    --res.quot;
  // TODO refactor this mess
  if (res.quot == 0 && index == 0)
  {
    if (current == end)
      ++res.quot;
    else
      res.quot = 2;
  }
  offset = std::min<std::uint64_t>(dist, offset + (3 * res.quot));
  std::advance(current, -offset);
  encoded = encode_base64_values(current, end);
  index = wrap_integer<0, 3>(n + index);
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
    _last_encoded_index = 0;
    _last_encoded = detail::encode_base64_values(_current_it, _end);
  }
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::begin() const
    -> iterator
{
  // FIXME enormous hack to avoid skipping the first 4 chars
  if (std::is_same<detail::iterator_category_t<
                       std::iterator_traits<underlying_iterator>>,
                   std::input_iterator_tag>::value)
  {
    return {*this};
  }
  return {{_begin, _end}};
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
  return _last_encoded->operator[](_last_encoded_index);
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

  if (_current_it == _end || _last_encoded_index == 4)
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
  if (lhs._last_encoded_index == 4|| rhs._last_encoded_index == 4)
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
