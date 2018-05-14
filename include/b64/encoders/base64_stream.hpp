#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <tuple>

#include <b64/detail/iterators/adaptive_iterator.hpp>
#include <b64/detail/meta/aliases.hpp>

#include <b64/detail/meta/concepts/iterable.hpp>
#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>

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
template <typename UnderlyingIterator,
          typename Sentinel = UnderlyingIterator,
          typename = std::enable_if_t<
              detail::is_iterator<UnderlyingIterator>::value &&
              detail::is_sentinel<Sentinel, UnderlyingIterator>::value &&
              detail::is_byte_integral<detail::value_type_t<
                  std::iterator_traits<UnderlyingIterator>>>::value>>
class base64_stream_encoder
{
  static constexpr char const alphabet[] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

  // FIXME the minimum level is forward, since multipass guarantee is on
  using iterator = detail::adaptive_iterator<
      base64_stream_encoder,
      detail::iterator_category_t<std::iterator_traits<UnderlyingIterator>>>;

public:
  using value_type = char;
  using difference_type = std::streamoff;

  base64_stream_encoder() = default;
  base64_stream_encoder(UnderlyingIterator const&, Sentinel const&);

  value_type const& get() const;
  std::size_t pos() const noexcept;
  void seek(difference_type);

  iterator begin() const
  {
    return {*this};
  }

  iterator end() const
  {
    // hack to trick the constructor
    // avoid encoding values twice
    base64_stream_encoder enc{_end, _end};
    enc._begin = _begin;
    // FIXME sentinel != iterator!!
    // TODO require that a default constructed encoder => begin() == end()
    enc._current_it = enc._end;
    enc._last_encoded_index = 4;
    return {enc};
  }

  template <typename T, typename U, typename V>
  friend bool operator==(base64_stream_encoder<T, U, V> const&,
                         base64_stream_encoder<T, U, V> const&);

private:
  void _encode_next_values();

  void _seek_forward(difference_type);
  void _seek_backward(difference_type);

  UnderlyingIterator _begin{};
  UnderlyingIterator _current_it{};
  Sentinel _end{};
  std::array<char, 4> _last_encoded;
  int _last_encoded_index{4};
};

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
    _last_encoded_index = 0;
    _encode_next_values();
  }
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
  return _last_encoded[_last_encoded_index];
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
std::size_t base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::pos()
    const noexcept
{
  // this function only gets called by adaptive_random_access_iterators
  // still using std::distance to avoid compiler errors on inferior iterators.
  if (_begin == _end)
    return 0;
  auto dist = std::distance(_begin, _current_it);
  assert(dist % 3 == 0 || (_current_it == _end));
  if (_current_it != _end)
    dist -= 3;
  auto const res = std::lldiv(dist, 3);
  return (res.quot + std::min(res.rem, 1ll)) * 4;
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::_seek_forward(
    difference_type n)
{
  assert(n > 0);
  auto max_increment = (_last_encoded_index + n) / 4;
  if (max_increment > 0)
  {
    if (max_increment > 1)
      max_increment--;
    std::advance(_current_it, (max_increment - 1) * 3);
    if (_current_it != _end)
    {
      _encode_next_values();
      _last_encoded_index = (_last_encoded_index + (n % 4)) % 4;
      return;
    }
  }

  _last_encoded_index += n % 4;
  if (_current_it == _end)
  {
    if (_last_encoded_index == 0)
      _last_encoded_index = 4;
  }
  else if (_last_encoded_index >= 4)
    _last_encoded_index %= 4;
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::
    _seek_backward(difference_type n)
{
  // TODO
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
void base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE>::seek(
    difference_type n)
{
  if (n > 0)
    _seek_forward(n);
  else if (n < 0)
    _seek_backward(n);
}

template <typename UnderlyingIterator, typename Sentinel, typename SFINAE>
bool operator==(
    base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE> const& lhs,
    base64_stream_encoder<UnderlyingIterator, Sentinel, SFINAE> const& rhs)
{
  // 4 can only be obtained by being at the end of encoding stream
  if (lhs._last_encoded_index == 4 &&
      lhs._last_encoded_index == rhs._last_encoded_index)
  {
    return true;
  }
  return std::tie(
             lhs._begin, lhs._current_it, lhs._end, lhs._last_encoded_index) ==
         std::tie(
             rhs._begin, rhs._current_it, rhs._end, rhs._last_encoded_index);
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
