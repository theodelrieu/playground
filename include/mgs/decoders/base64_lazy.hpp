#pragma once

#include <array>
#include <type_traits>

#include <mgs/detail/iterators/adaptive_iterator.hpp>
#include <mgs/detail/meta/concepts/byte_integral.hpp>
#include <mgs/detail/meta/concepts/input_iterator.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>

namespace mgs
{
namespace decoders
{
template <typename UnderlyingIterator,
          typename Sentinel = UnderlyingIterator,
          typename = std::enable_if_t<
              detail::is_input_iterator<UnderlyingIterator>::value &&
              detail::is_sentinel<Sentinel, UnderlyingIterator>::value &&
              detail::is_byte_integral<detail::value_type_t<
                  std::iterator_traits<UnderlyingIterator>>>::value>>
class base64_lazy_decoder
{
private:
  using iterator_category = std::input_iterator_tag;
  using iterator =
      detail::adaptive_iterator<base64_lazy_decoder, iterator_category>;

public:
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

  using value_type = std::uint8_t;
  using difference_type = std::streamoff;

  base64_lazy_decoder() = default;
  base64_lazy_decoder(UnderlyingIterator const& begin, Sentinel const& end);

  value_type get() const;
  void seek_forward(difference_type n);

  iterator begin() const;
  iterator end() const;

private:
  void _decode_next_values();

  UnderlyingIterator _current{};
  Sentinel _end{};
  char _max_index{3};
  char _index{_max_index};
  std::array<std::uint8_t, 3> _decoded;

  template <typename T, typename U, typename V>
  friend bool operator==(base64_lazy_decoder<T, U, V> const& lhs,
                         base64_lazy_decoder<T, U, V> const& rhs);
};
}
}

#include <mgs/detail/decoders/base64_lazy_impl.hpp>
