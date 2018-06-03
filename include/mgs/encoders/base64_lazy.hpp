#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <iterator>
#include <tuple>

#include <mgs/detail/iterators/adaptive_iterator.hpp>
#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/byte_integral.hpp>
#include <mgs/detail/meta/concepts/input_iterator.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>

namespace mgs
{
namespace encoders
{
template <typename UnderlyingIterator,
          typename Sentinel = UnderlyingIterator,
          typename = std::enable_if_t<
              detail::is_input_iterator<UnderlyingIterator>::value &&
              detail::is_sentinel<Sentinel, UnderlyingIterator>::value &&
              detail::is_byte_integral<detail::value_type_t<
                  std::iterator_traits<UnderlyingIterator>>>::value>>
class base64_lazy_encoder
{
private:
  using iterator_category = std::input_iterator_tag;
  using iterator =
      detail::adaptive_iterator<base64_lazy_encoder, iterator_category>;

public:
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

  using value_type = char;
  using difference_type = std::streamoff;

  base64_lazy_encoder() = default;
  base64_lazy_encoder(UnderlyingIterator const& begin, Sentinel const& end);

  value_type const& get() const;
  void seek_forward(difference_type n);

  iterator begin() const;
  iterator end() const;

private:
  UnderlyingIterator _current{};
  Sentinel _end{};
  int _index{4};
  std::array<char, 4> _encoded;

  template <typename T, typename U, typename V>
  friend bool operator==(base64_lazy_encoder<T, U, V> const& lhs,
                         base64_lazy_encoder<T, U, V> const& rhs);
};
}
}

#include <mgs/detail/encoders/base64_lazy_impl.hpp>
