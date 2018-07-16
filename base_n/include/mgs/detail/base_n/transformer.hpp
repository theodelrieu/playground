#pragma once

#include <array>
#include <cstdlib>
#include <type_traits>

#include <mgs/detail/base_n/math.hpp>
#include <mgs/detail/iterators/adaptive_iterator.hpp>
#include <mgs/detail/meta/concepts/byte_integral.hpp>
#include <mgs/detail/meta/concepts/input_iterator.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
// TODO sfinae on encoding traits
template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel = UnderlyingIterator,
          typename = std::enable_if_t<
              detail::is_input_iterator<UnderlyingIterator>::value &&
              detail::is_sentinel<Sentinel, UnderlyingIterator>::value &&
              detail::is_byte_integral<detail::value_type_t<
                  std::iterator_traits<UnderlyingIterator>>>::value>>
class base_n_transformer
{
  using iterator =
      adaptive_iterator<base_n_transformer, std::input_iterator_tag>;

public:
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

  using difference_type = typename EncodingTraits::difference_type;
  using value_type = typename EncodingTraits::value_type;

  base_n_transformer() = default;
  base_n_transformer(UnderlyingIterator const& begin, Sentinel const& end);

  value_type get() const;
  void seek_forward(difference_type n);

  iterator begin() const;
  iterator end() const;

private:
  static constexpr auto nb_output_bytes =
      EncodingTraits::algorithm::nb_output_bytes;

  UnderlyingIterator _current{};
  Sentinel _end{};
  std::array<value_type, nb_output_bytes> _output;
  // only useful for decoding, but it allows to have the same class for both
  char _max_index{nb_output_bytes};
  char _index{_max_index};

  void _process_input();

  template <typename T, typename U, typename V, typename W>
  friend bool operator==(base_n_transformer<T, U, V, W> const& lhs,
                         base_n_transformer<T, U, V, W> const& rhs);
};
}
}
}

#include <mgs/detail/base_n/transformer_impl.hpp>
