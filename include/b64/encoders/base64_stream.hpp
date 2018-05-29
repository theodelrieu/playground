#pragma once

#include <array>

#include <b64/detail/iterators/adaptive_iterator.hpp>
#include <b64/detail/meta/aliases.hpp>

#include <b64/detail/encoders/base64_stream_impl.hpp>
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
// Must have this, since inheritance errors are not SFINAE.
template <typename Iterator, typename Sentinel>
struct base64_stream_encoder_requirements
{
  static constexpr auto value =
      ((detail::is_input_iterator<Iterator>::value &&
        detail::is_sentinel<Sentinel, Iterator>::value) ||
       (detail::is_random_access_iterator<Iterator>::value &&
        detail::is_sized_sentinel<Sentinel, Iterator>::value)) &&
      detail::is_byte_integral<
          detail::value_type_t<std::iterator_traits<Iterator>>>::value;
};
}

namespace encoders
{
// TODO test static_assert with valid RandomAccessIterator and Sentinel (not SizedSentinel)
// to be sure SFINAE correctness is not broken.
template <typename UnderlyingIterator,
          typename Sentinel = UnderlyingIterator,
          typename = std::enable_if_t<
              detail::base64_stream_encoder_requirements<UnderlyingIterator,
                                                         Sentinel>::value>>
class base64_stream_encoder
    : private detail::base64_stream_encoder_impl<
          base64_stream_encoder<UnderlyingIterator, Sentinel>>
{
  using base = detail::base64_stream_encoder_impl<base64_stream_encoder<UnderlyingIterator, Sentinel>>;

public:
  using base::base;

  using typename base::value_type;
  using typename base::difference_type;

  using typename base::iterator;

  using typename base::underlying_iterator;
  using typename base::underlying_sentinel;

  using base::get;
  using base::seek_forward;
  using base::seek_backward;
  using base::pos;

  using base::begin;
  using base::end;

  template <typename T, typename U, typename V>
  friend bool operator==(base64_stream_encoder<T, U, V> const& lhs,
                         base64_stream_encoder<T, U, V> const& rhs)
  {
    return static_cast<base const&>(lhs) == static_cast<base const&>(rhs);
  }
};

template <typename T, typename U, typename V>
bool operator!=(base64_stream_encoder<T, U, V> const& lhs,
                base64_stream_encoder<T, U, V> const& rhs)
{
  return !(lhs == rhs);
}
}
}

