#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <iterator>
#include <tuple>

#include <b64/detail/advance_to.hpp>
#include <b64/detail/distance.hpp>
#include <b64/detail/iterators/adaptive_iterator.hpp>
#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/bidirectional_iterator.hpp>
#include <b64/detail/meta/concepts/forward_iterator.hpp>
#include <b64/detail/meta/concepts/input_iterator.hpp>
#include <b64/detail/meta/concepts/random_access_iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>
#include <b64/detail/meta/concepts/sized_sentinel.hpp>
#include <b64/detail/wrap_integer.hpp>

void foo()
{
}

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
    return encoded;
  }
};

template <typename UnderlyingIterator,
          typename Sentinel = UnderlyingIterator,
          typename IteratorTag =
              iterator_category_t<std::iterator_traits<UnderlyingIterator>>,
          typename = void>
struct base64_stream_encoder_impl;

template <typename UnderlyingIterator, typename Sentinel>
class base64_stream_encoder_impl<
    UnderlyingIterator,
    Sentinel,
    std::input_iterator_tag,
    std::enable_if_t<is_input_iterator<UnderlyingIterator>::value &&
                     is_sentinel<Sentinel, UnderlyingIterator>::value &&
                     is_byte_integral<value_type_t<
                         std::iterator_traits<UnderlyingIterator>>>::value>>
{
  using self = base64_stream_encoder_impl;

protected:
  using algorithm = base64_encode_algorithm;
  using iterator_category = std::input_iterator_tag;

public:
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

  self begin_impl() const
  {
    return *this;
  }

  self end_impl() const
  {
    // hack to trick the constructor, avoid encoding values twice
    // no need to set current to end, since _index == 4
    // This only holds true for input/forward iterators,
    // bidirectional iterators can be decremented, so _current == _end for those
    // iterators
    base64_stream_encoder_impl enc;
    enc._current = _current;
    enc._end = _end;
    assert(enc._index == 4);
    return enc;
  }

protected:
  UnderlyingIterator _current{};
  Sentinel _end{};
  int _index{4};
  std::array<char, 4> _encoded;

  template <typename T, typename U, typename V>
  friend bool operator==(
      base64_stream_encoder_impl<T, U, std::input_iterator_tag, V> const& lhs,
      base64_stream_encoder_impl<T, U, std::input_iterator_tag, V> const& rhs)
  {
    if (lhs._index == 4 || rhs._index == 4)
      return lhs._index == rhs._index;
    return std::tie(lhs._current, lhs._index) ==
           std::tie(rhs._current, rhs._index);
  }
};

template <typename UnderlyingIterator, typename Sentinel>
class base64_stream_encoder_impl<
    UnderlyingIterator,
    Sentinel,
    std::forward_iterator_tag,
    std::enable_if_t<is_forward_iterator<UnderlyingIterator>::value &&
                     is_sentinel<Sentinel, UnderlyingIterator>::value &&
                     is_byte_integral<value_type_t<
                         std::iterator_traits<UnderlyingIterator>>>::value>>
  : public base64_stream_encoder_impl<UnderlyingIterator,
                                      Sentinel,
                                      std::input_iterator_tag>
{
  using self = base64_stream_encoder_impl;

protected:
  using base = base64_stream_encoder_impl<UnderlyingIterator,
                                          Sentinel,
                                          std::input_iterator_tag>;

  using iterator_category = std::forward_iterator_tag;

public:
  base64_stream_encoder_impl() = default;
  base64_stream_encoder_impl(UnderlyingIterator const& begin,
                             Sentinel const& end)
    : base(begin, end), _begin(begin)
  {
  }

  self begin_impl() const
  {
    return {_begin, this->_end};
  }

  self end_impl() const
  {
    self enc;
    enc._begin = _begin;
    enc._current = _begin;
    enc._end = this->_end;
    return enc;
  }

protected:
  UnderlyingIterator _begin{};

  template <typename T,
            typename U,
            typename V,
            typename W,
            std::enable_if_t<
                detail::is_derived_from<V, std::forward_iterator_tag>::value>>
  friend bool operator==(base64_stream_encoder_impl<T, U, V, W> const& lhs,
                         base64_stream_encoder_impl<T, U, V, W> const& rhs)
  {

    if (lhs._index == 4 || rhs._index == 4)
      return lhs._index == rhs._index;
    return std::tie(lhs._current, lhs._index, lhs._begin) ==
           std::tie(rhs._current, rhs._index, rhs._begin);
  }
};

template <typename UnderlyingIterator, typename Sentinel>
class base64_stream_encoder_impl<
    UnderlyingIterator,
    Sentinel,
    std::bidirectional_iterator_tag,
    std::enable_if_t<is_bidirectional_iterator<UnderlyingIterator>::value &&
                     is_sentinel<Sentinel, UnderlyingIterator>::value &&
                     is_byte_integral<value_type_t<
                         std::iterator_traits<UnderlyingIterator>>>::value>>
  : public base64_stream_encoder_impl<UnderlyingIterator,
                                      Sentinel,
                                      std::forward_iterator_tag>
{
  using self = base64_stream_encoder_impl;

protected:
  using base = base64_stream_encoder_impl<UnderlyingIterator,
                                          Sentinel,
                                          std::forward_iterator_tag>;

  using iterator_category = std::bidirectional_iterator_tag;

public:
  using base::base;

  self begin_impl() const
  {
    return static_cast<self&&>(base::begin_impl());
  }

  self end_impl() const
  {
    self enc;
    enc._begin = this->_begin;
    enc._current = this->_current;
    enc._end = this->_end;
    ::b64::detail::advance_to(enc._current, enc._end);
    return enc;
  }

  void seek_backward(typename base::difference_type n)
  {
    foo();
    assert(n == -1);

    auto offset = decltype(n){0};
    if (this->_current != this->_end)
    {
      assert(this->_index != 4);
      this->_index = detail::wrap_integer<0, 3>(this->_index - 1);
      if (this->_index != 3)
        return;
      offset = 3 * 2;
    }
    else
    {
      offset = ::b64::detail::distance(this->_begin, this->_end) % 3;
      if (offset == 0)
        offset = 3;
      // at the very end
      if (this->_index == 4)
        this->_index = 3;
      else
      {
        this->_index = detail::wrap_integer<0, 3>(this->_index - 1);
        if (this->_index == 3)
          offset += 3;
      }
    }
    assert(offset);
    std::advance(this->_current, -offset);
    this->_encoded = typename base::algorithm{}(this->_current, this->_end);
  }
};

template <typename UnderlyingIterator, typename Sentinel>
class base64_stream_encoder_impl<
    UnderlyingIterator,
    Sentinel,
    std::random_access_iterator_tag,
    std::enable_if_t<is_random_access_iterator<UnderlyingIterator>::value &&
                     is_sized_sentinel<Sentinel, UnderlyingIterator>::value &&
                     is_byte_integral<value_type_t<
                         std::iterator_traits<UnderlyingIterator>>>::value>>
  : public base64_stream_encoder_impl<UnderlyingIterator,
                                      Sentinel,
                                      std::bidirectional_iterator_tag>
{
  using self = base64_stream_encoder_impl;

protected:
  using base = base64_stream_encoder_impl<UnderlyingIterator,
                                          Sentinel,
                                          std::bidirectional_iterator_tag>;

  using iterator_category = std::random_access_iterator_tag;

public:
  using base::base;

  self begin_impl() const
  {
    return static_cast<self&&>(base::begin_impl());
  }

  self end_impl() const
  {
    return static_cast<self&&>(base::end_impl());
  }

  void seek_forward(typename base::difference_type n)
  {
    assert(n > 0);
    assert(this->_index != 4);

    if (this->_index + n <= 3)
    {
      this->_index += n;
      return;
    }

    auto const offset = n - (4 - this->_index);
    auto const res = std::lldiv(offset, 4);
    auto const dist = this->_end - this->_current;

    this->_current += std::min<std::uint64_t>(dist, res.quot * 3);
    this->_index = detail::wrap_integer<0, 3>(this->_index + n);

    if (this->_current == this->_end)
    {
      if (res.rem == 0)
      {
        this->_index = 4;
        return;
      }
      this->_current -= dist % 3;
    }
    this->_encoded = typename base::algorithm{}(this->_current, this->_end);
  }

  void seek_backward(typename base::difference_type n)
  {
    assert(n < 0);

    if (this->_index != 4 && this->_index + n >= 0)
    {
      this->_index += n;
      return;
    }

    if (this->_index == 4)
      this->_current = this->_begin + (this->_end - this->_begin);
    auto offset = decltype(n)(0);
    auto const dist = this->_current - this->_begin;
    if (this->_current == this->_end)
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
    if (res.quot == 0 && this->_index == 0)
    {
      if (this->_current == this->_end)
        ++res.quot;
      else
        res.quot = 2;
    }
    offset = std::min<std::uint64_t>(dist, offset + (3 * res.quot));
    std::advance(this->_current, -offset);
    this->_encoded = typename base::algorithm{}(this->_current, this->_end);
    this->_index = detail::wrap_integer<0, 3>(n + this->_index);
  }

  typename base::difference_type pos() const noexcept
  {
    auto dist = typename base::difference_type{0};

    if ((this->_current == this->_end) || this->_index == 4)
      dist = this->_end - this->_begin;
    else
    {
      dist = this->_current - this->_begin;
      // FIXME clean this code...
      if ((this->_current != this->_end) && dist > 0)
        dist -= 3;
    }

    auto const res = std::lldiv(dist, 3);
    return (res.quot + std::min(res.rem, 1ll)) * 4;
  }
};

template <typename T, typename U, typename V, typename W>
bool operator!=(
    base64_stream_encoder_impl<T, U, V, W> const& lhs,
    base64_stream_encoder_impl<T, U, V, W> const& rhs)
{
  return !(lhs == rhs);
}
}
}
