#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <limits>
#include <utility>

#include <mgs/codecs/binary_to_text/concepts/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/bitset_utils.hpp>
#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>
#include <mgs/codecs/binary_to_text/detail/output_encoder.hpp>
#include <mgs/codecs/binary_to_text/detail/padding_writer.hpp>
#include <mgs/codecs/binary_to_text/detail/span.hpp>
#include <mgs/codecs/binary_to_text/detail/static_vector.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace binary_to_text
{
template <typename Iterator, typename Sentinel, typename EncodingTraits>
class basic_encoder
{
private:
  static_assert(meta::concepts::iterator::is_input_iterator<Iterator>::value,
                "Iterator is not an InputIterator");
  static_assert(
      meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value,
      "Sentinel is not a Sentinel<Iterator>");

  using _ = concepts::trigger_static_asserts<EncodingTraits>;

  static_assert(EncodingTraits::padding_policy != padding_policy::optional,
                "optional padding does not make sense when encoding");

  using BitshiftTraits =
      detail::bitshift_traits<EncodingTraits::nb_input_bytes,
                              EncodingTraits::nb_output_bytes>;

  static_assert(BitshiftTraits::nb_total_input_bits % BitshiftTraits::nb_output_bytes == 0,
                "The impossible has occurred");
  static_assert(detail::pow<2, BitshiftTraits::nb_encoded_bits>() ==
                    sizeof(EncodingTraits::alphabet),
                "Alphabet size must be a power of 2");
  static_assert(BitshiftTraits::nb_output_bytes % 2 == 0,
                "nb_output_bytes must be a power of two");

  static constexpr auto nb_bytes_to_read =
      (256 / BitshiftTraits::nb_output_bytes) * BitshiftTraits::nb_input_bytes;

public:
  using value_type = detail::static_vector<char, 256>;
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;

  basic_encoder() = default;

  basic_encoder(Iterator begin, Sentinel end)
    : _current(std::move(begin)), _end(std::move(end))
  {
  }

  void operator()(value_type& out)
  {
    if (_current != this->_end)
      read_input(out);
    else
      out.resize(0);
  }

private:
  template <typename I, typename S>
  auto read_input_impl(I& current, S end, std::input_iterator_tag)
  {
    detail::static_vector<unsigned char, nb_bytes_to_read> ret;
    auto i = 0;
    for (; i < nb_bytes_to_read; ++i)
    {
      if (current == end)
        break;
      ret.push_back(*current++);
    }
    return ret;
  }

  template <typename I, typename S>
  auto read_input_impl(I& current, S end, std::random_access_iterator_tag)
  {
    auto const dist = std::min(static_cast<long int>(nb_bytes_to_read), end - current);

    detail::span<I> s{current, current + dist};
    current += dist;
    return s;
  }

  void read_input(value_type& output)
  {
    auto const input = read_input_impl(
        _current,
        _end,
        typename std::iterator_traits<Iterator>::iterator_category{});

    constexpr std::bitset<BitshiftTraits::nb_total_input_bits> mask(
        detail::pow<2, BitshiftTraits::nb_encoded_bits>() - 1);

    auto const nb_loop_iterations =
        std::ldiv(input.size(), BitshiftTraits::nb_input_bytes);
    output.resize(nb_loop_iterations.quot * BitshiftTraits::nb_output_bytes);

    for (auto i = 0u; i < nb_loop_iterations.quot; ++i)
    {
      auto const input_bits = detail::input_to_bitset<BitshiftTraits>(
          input.begin() + (i * BitshiftTraits::nb_input_bytes),
          BitshiftTraits::nb_input_bytes);

      detail::output_encoder<EncodingTraits>::encode(
          input_bits, output.begin() + (i * BitshiftTraits::nb_output_bytes));
    }

    if (nb_loop_iterations.rem)
    {
      auto const nb_non_padded_bytes =
          ((8 * (nb_loop_iterations.rem)) / BitshiftTraits::nb_encoded_bits) +
          1;
      auto const nb_padded_bytes =
          BitshiftTraits::nb_output_bytes - nb_non_padded_bytes;

      auto const input_bits = detail::input_to_bitset<BitshiftTraits>(
          input.begin() +
              (nb_loop_iterations.quot * BitshiftTraits::nb_input_bytes),
          nb_loop_iterations.rem);

      auto const old_size = output.size();
      output.resize(output.size() + nb_non_padded_bytes);
      detail::output_encoder<EncodingTraits>::encode(
          input_bits, output.begin() + old_size, nb_non_padded_bytes);
      detail::padding_writer<EncodingTraits>::write(std::back_inserter(output),
                                                    nb_padded_bytes);
    }
  }

  template <typename I, typename S, typename T>
  friend bool operator==(basic_encoder<I, S, T> const&,
                         basic_encoder<I, S, T> const&);

  Iterator _current{};
  Sentinel _end{};
};

template <typename I, typename S, typename T>
bool operator==(basic_encoder<I, S, T> const& lhs,
                basic_encoder<I, S, T> const& rhs)
{
  return (lhs._current == lhs._end || rhs._current == rhs._end) ?
             (lhs._current == lhs._end && rhs._current == rhs._end) :
             lhs._current == rhs._current;
}

template <typename I, typename S, typename T>
bool operator!=(basic_encoder<I, S, T> const& lhs,
                basic_encoder<I, S, T> const& rhs)
{
  return !(lhs == rhs);
}
}
}
}
}
