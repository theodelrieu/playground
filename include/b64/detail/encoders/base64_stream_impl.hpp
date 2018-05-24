#pragma once

#include <algorithm>
#include <bitset>
#include <cassert>

#include <b64/detail/meta/concepts/sized_sentinel.hpp>

namespace b64
{
namespace encoders
{
template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
char const
    base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::alphabet[];

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
    _encode_next_values();
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
  // _last_encoded_index == nullopt
  base64_stream_encoder enc;
  enc._begin = _begin;
  enc._current_it = _begin;
  enc._end = _end;
  return {std::move(enc)};
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
    _encode_next_values()
{
  assert(_current_it != _end);

  std::bitset<24> bits;
  int i = 0;
  for (; i < 3; ++i)
  {
    if (_current_it == _end)
      break;
    auto byte = static_cast<std::uint8_t>(*_current_it++);
    bits |= (byte << (16 - (8 * i)));
  }

  for (int j = 0; j < i + 1; ++j)
  {
    auto shift = (18 - (6 * j));
    auto mask = ((std::bitset<24>(0xFFFFFF) >> 18) << shift);
    auto l = bits & mask;
    l >>= shift;
    auto index = static_cast<std::uint8_t>(l.to_ulong());
    _last_encoded[j] = alphabet[index];
  }
  std::fill(std::next(_last_encoded.begin(), i + 1), _last_encoded.end(), '=');
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
auto base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::get() const
    -> value_type const&
{
  return _last_encoded[*_last_encoded_index];
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
  assert(n > 0);
  auto const res = std::ldiv(*_last_encoded_index + n, 4);
  auto const max_increment = res.quot;
  if (max_increment == 0)
    *_last_encoded_index += n;
  else
  {
    // if _last_encoded_index + n is a multiple of 4, it could reach the end.
    // However, we cannot know that before we increment the iterator.
    // In this specific case, perform 2 _encode_next_values() and test for
    // _current_it == _end each time.
    auto const min_increment =
        (max_increment > 1 ? max_increment - (res.rem == 0 ? 2 : 1) : 0);
    std::advance(_current_it, min_increment * 3);
    if (_current_it == _end)
      _last_encoded_index = nonstd::nullopt;
    else
    {
      // Must encode values here, even if we'll reach the end.
      // This is because we cannot assume the underlying iterator category
      // here. So going backwards is not an option.
      _encode_next_values();
      *_last_encoded_index += n;
      if (max_increment - min_increment == 2)
      {
        if (_current_it != _end)
          _encode_next_values();
        else if (_last_encoded_index == 0)
          _last_encoded_index = nonstd::nullopt;
      }
    }
  }
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
template <typename, typename>
void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
    seek_backward(difference_type n)
{
  assert(n < 0);
  auto const idx = (_last_encoded_index ? *_last_encoded_index + n : 4 + n);
  _last_encoded_index.emplace(idx);
  if (idx >= 0)
    return;

  auto res = std::ldiv(idx, 4);
  if (_current_it == _end)
  {
    // we have to reset the underlying iterator so that
    // distance(begin, current) % 3 == 0
    auto const nb_read =
        3 - std::count(_last_encoded.begin() + 2, _last_encoded.end(), '=');
    std::advance(_current_it, -nb_read);
  }
  else
    res.quot--;
  if (res.rem)
    res.quot--;
  std::advance(_current_it, res.quot * 3);
  _encode_next_values();
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
