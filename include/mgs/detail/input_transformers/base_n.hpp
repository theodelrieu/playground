#pragma once

#include <array>
#include <type_traits>

#include <mgs/detail/meta/concepts/byte_integral.hpp>
#include <mgs/detail/meta/concepts/input_iterator.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>

namespace mgs
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
private:
  using iterator_category = std::input_iterator_tag;

public:
  using encoding_traits = EncodingTraits;
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

  using typename encoding_traits::value_type;
  using typename encoding_traits::difference_type;

  base_n_transformer() = default;
  base_n_transformer(UnderlyingIterator const& begin, Sentinel const& end);

  value_type get() const;
  void seek_forward(difference_type n);

private:
  UnderlyingIterator _current{};
  Sentinel _end{};
  char _index{encoding_traits::nb_output_bytes};
  std::array<value_type, encoding_traits::nb_output_bytes> _encoded;

  template <typename T, typename U, typename V, typename W>
  friend bool operator==(base_n_transformer<T, U, V, W> const& lhs,
                         base_n_transformer<T, U, V, W> const& rhs);

  template <typename T, typename U, typename V, typename W, typename>
  friend auto end(base_n_transformer<T, U, V, W> const&);
};

template <typename T, typename U, typename V, typename W, typename = void>
auto begin(base_n_transformer<T, U, V, W> const&);
}
}

#include <mgs/detail/input_transformers/base_n_impl.hpp>
