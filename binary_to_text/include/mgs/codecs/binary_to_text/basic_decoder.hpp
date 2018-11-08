#pragma once

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <limits>

#include <mgs/codecs/binary_to_text/concepts/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/bitset_utils.hpp>
#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/encoded_input_reader.hpp>
#include <mgs/codecs/binary_to_text/detail/input_sanitizer.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>
#include <mgs/codecs/binary_to_text/detail/output_decoder.hpp>
#include <mgs/codecs/binary_to_text/detail/span.hpp>
#include <mgs/codecs/binary_to_text/detail/static_vector.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/static_asserts.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace binary_to_text
{
template <typename Iterator, typename Sentinel, typename EncodingTraits>
class basic_decoder
{
private:
  static constexpr auto _ =
      meta::trigger_static_asserts<
          meta::concepts::iterator::is_input_iterator<Iterator>>() &&
      meta::trigger_static_asserts<
          meta::concepts::iterator::is_sentinel<Sentinel, Iterator>>() &&
      meta::trigger_static_asserts<
          concepts::is_encoding_traits<EncodingTraits>>();

  using BitshiftTraits = detail::bitshift_traits<EncodingTraits>;

  static constexpr auto nb_bytes_to_read =
      (256 / BitshiftTraits::nb_encoded_bytes) *
      BitshiftTraits::nb_encoded_bytes;
  static_assert(nb_bytes_to_read % BitshiftTraits::nb_encoded_bytes == 0, "The impossible has occurred");

public:
  using value_type = detail::static_vector<std::uint8_t, 256>;
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;
  using encoding_traits = EncodingTraits;

  basic_decoder() = default;

  basic_decoder(Iterator begin, Sentinel end)
    : _current(std::move(begin)), _end(std::move(end))
  {
  }

  void operator()(value_type& output)
  {
    output.resize(0);
    if (_current != _end)
      read_input(output);
  }

  template <typename I, typename S>
  auto read_input_impl(I& current, S end, std::input_iterator_tag)
  {
    detail::static_vector<std::uint8_t, nb_bytes_to_read> ret;
    ret.resize(nb_bytes_to_read);

    auto i = 0;
    for (; i < nb_bytes_to_read; ++i)
    {
      if (current == end)
        break;
      auto const byte = *current++;
      ret[i] = byte;
    }
    ret.resize(i);
    auto const sanitized_size =
        detail::input_sanitizer<EncodingTraits>::sanitize(ret, current == end);
    ret.resize(sanitized_size);
    return ret;
  }

  template <typename I, typename S>
  auto read_input_impl(I& current, S end, std::random_access_iterator_tag)
  {
    auto const dist =
        std::min(static_cast<long int>(nb_bytes_to_read), end - current);

    auto const input_end = current + dist;
    auto const sanitized_size =
        detail::input_sanitizer<EncodingTraits>::sanitize(
            detail::span<I, S>{current, input_end}, input_end == end);
    detail::span<I, S> sanitized_input{current, current + sanitized_size};
    current += dist;
    return sanitized_input;
  }

  void read_input(value_type& output)
  {
    auto const input = read_input_impl(
        _current,
        _end,
        typename std::iterator_traits<Iterator>::iterator_category{});

    auto const nb_loop_iterations =
        std::ldiv(input.size(), BitshiftTraits::nb_encoded_bytes);

    output.resize(nb_loop_iterations.quot * BitshiftTraits::nb_decoded_bytes);
    for (auto i = 0; i < nb_loop_iterations.quot; ++i)
    {
      auto const indices_bits = detail::indices_to_bitset<EncodingTraits>(
          input.begin() + (i * BitshiftTraits::nb_encoded_bytes),
          BitshiftTraits::nb_encoded_bytes);

      detail::output_decoder<EncodingTraits>::decode(
          indices_bits,
          output.begin() + (i * BitshiftTraits::nb_decoded_bytes));
    }
    if (nb_loop_iterations.rem)
    {
      auto const nb_last_bytes_to_decode = (nb_loop_iterations.rem * BitshiftTraits::nb_index_bits) / 8;
      auto const old_size = output.size();
      output.resize(old_size + nb_last_bytes_to_decode);

      auto const indices_bits = detail::indices_to_bitset<EncodingTraits>(
          input.begin() +
              (nb_loop_iterations.quot * BitshiftTraits::nb_encoded_bytes),
          nb_loop_iterations.rem);

      detail::output_decoder<EncodingTraits>::decode(
          indices_bits, output.begin() + old_size, nb_last_bytes_to_decode);
    }
  }

  Iterator _current{};
  Sentinel _end{};
};
}
}
}
}
