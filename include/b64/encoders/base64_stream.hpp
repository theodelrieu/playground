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
namespace encoders
{
// TODO test static_assert with valid RandomAccessIterator and Sentinel (not SizedSentinel)
// to be sure SFINAE correctness is not broken.
template <
    typename UnderlyingIterator,
    typename Sentinel = UnderlyingIterator,
    typename = void>
class base64_stream_encoder;

template <typename UnderlyingIterator, typename Sentinel>
class base64_stream_encoder<
    UnderlyingIterator,
    Sentinel,
    // every SFINAE check is done in the base class, but we have to trigger SFINAE here.
    // inheritance errors are not SFINAE.
    decltype(std::declval<detail::base64_stream_encoder_impl<UnderlyingIterator,
                                                             Sentinel>>(),
             void())>
  : public detail::base64_stream_encoder_impl<UnderlyingIterator, Sentinel>
{
  using base = detail::base64_stream_encoder_impl<UnderlyingIterator, Sentinel>;
  using iterator = detail::adaptive_iterator<base64_stream_encoder,
                                             detail::iterator_category_t<base>>;

public:
  using base::base;

  using typename base::value_type;
  using typename base::difference_type;

  using typename base::underlying_iterator;
  using typename base::underlying_sentinel;

  iterator begin() const
  {
    auto enc_base = (static_cast<base const*>(this))->begin_impl();
    return {static_cast<base64_stream_encoder const&>(enc_base)};
  }

  iterator end() const
  {
    auto enc_base = (static_cast<base const*>(this))->end_impl();
    return {static_cast<base64_stream_encoder const&>(enc_base)};
  }
};

template <typename T, typename U, typename V>
bool operator==(base64_stream_encoder<T, U, V> const& lhs,
                base64_stream_encoder<T, U, V> const& rhs)
{
  using base = detail::base64_stream_encoder_impl<T, U>;

  return static_cast<base const&>(lhs) == static_cast<base const&>(rhs);
}

template <typename T, typename U, typename V>
bool operator!=(base64_stream_encoder<T, U, V> const& lhs,
                base64_stream_encoder<T, U, V> const& rhs)
{
  return !(lhs == rhs);
}
}
}

