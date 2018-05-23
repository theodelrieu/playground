#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>

#include <optional.hpp>

#include <b64/detail/iterators/adaptive_iterator.hpp>
#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/wrap_integer.hpp>

#include <b64/detail/meta/concepts/encoder.hpp>
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

  using iterator = detail::adaptive_iterator<
      base64_stream_encoder,
      detail::iterator_category_t<std::iterator_traits<UnderlyingIterator>>>;

public:
  using value_type = char;
  using difference_type = std::streamoff;
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

  base64_stream_encoder() = default;
  base64_stream_encoder(UnderlyingIterator const&, Sentinel const&);

  value_type const& get() const;
  std::size_t pos() const noexcept;
  void seek_forward(difference_type);
  void seek_backward(difference_type);

  iterator begin() const;
  iterator end() const;

  template <typename T, typename U, typename V>
  friend bool operator==(base64_stream_encoder<T, U, V> const&,
                         base64_stream_encoder<T, U, V> const&);

private:
  void _encode_next_values();

  UnderlyingIterator _begin{};
  UnderlyingIterator _current_it{};
  Sentinel _end{};
  std::array<char, 4> _last_encoded;
  nonstd::optional<detail::wrap_integer<0, 3>> _last_encoded_index;
};
}
}

#include <b64/detail/encoders/base64_stream_impl.hpp>

static_assert(
    b64::detail::is_encoder<b64::encoders::base64_stream_encoder<char*>>::value,
    "");
