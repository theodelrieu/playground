#pragma once

#include <array>

#include <b64/detail/iterators/adaptive_iterator.hpp>
#include <b64/detail/meta/aliases.hpp>

#include <b64/detail/meta/concepts/bidirectional_iterator.hpp>
#include <b64/detail/meta/concepts/iterable.hpp>
#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/random_access_iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>
#include <b64/detail/meta/concepts/sized_sentinel.hpp>

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
template <
    typename UnderlyingIterator,
    typename Sentinel = UnderlyingIterator,
    typename = std::enable_if_t<
        ((detail::is_input_iterator<UnderlyingIterator>::value &&
          detail::is_sentinel<Sentinel, UnderlyingIterator>::value) ||
         (detail::is_random_access_iterator<UnderlyingIterator>::value &&
          detail::is_sized_sentinel<Sentinel, UnderlyingIterator>::value)) &&
        detail::is_byte_integral<detail::value_type_t<
            std::iterator_traits<UnderlyingIterator>>>::value>>
class base64_stream_encoder
{
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

  void seek_forward(difference_type);

  iterator begin() const;
  iterator end() const;

  template <
      typename T = UnderlyingIterator,
      typename = std::enable_if_t<detail::is_bidirectional_iterator<T>::value>>
  void seek_backward(difference_type);

  template <
      typename T = UnderlyingIterator,
      typename = std::enable_if_t<detail::is_random_access_iterator<T>::value>>
  auto pos() const noexcept -> difference_type;

  template <typename T, typename U, typename V>
  friend bool operator==(base64_stream_encoder<T, U, V> const&,
                         base64_stream_encoder<T, U, V> const&);

private:
  UnderlyingIterator _begin{};
  UnderlyingIterator _current_it{};
  Sentinel _end{};
  std::array<char, 4> _last_encoded;
  int _last_encoded_index{4};
};
}
}

#include <b64/detail/encoders/base64_stream_impl.hpp>
