#pragma once

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <limits>

#include <mgs/codecs/binary_to_text/concepts/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/encoded_input_reader.hpp>
#include <mgs/codecs/binary_to_text/detail/invalid_character_handler.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>
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
class basic_decoder
{
private:
  static_assert(meta::concepts::iterator::is_input_iterator<Iterator>::value,
                "Iterator is not an InputIterator");
  static_assert(
      meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value,
      "Sentinel is not a Sentinel<Iterator>");

  using _ = concepts::trigger_static_asserts<EncodingTraits>;

  using BitshiftTraits = detail::bitshift_traits<EncodingTraits>;

  static_assert(BitshiftTraits::nb_total_encoded_bits %
                        BitshiftTraits::nb_decoded_bytes ==
                    0,
                "The impossible has occurred");

  static constexpr auto nb_bytes_to_read =
      256 - (BitshiftTraits::nb_index_bits / 8);

public:
  using value_type =
      detail::static_vector<std::uint8_t, BitshiftTraits::nb_decoded_bytes>;
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;

  basic_decoder() = default;

  basic_decoder(Iterator begin, Sentinel end)
    : _current(std::move(begin)), _end(std::move(end))
  {
  }

  void operator()(value_type& output)
  {
    output.resize(0);
    if (_current != _end)
    {
      read_input(output);
      // auto const res = read();
      //
      // decode_input_bits(res, std::back_inserter(output));
    }
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

      if (EncodingTraits::find_char(byte) != -1)
        ret[i] = byte;
      else
      {
        detail::invalid_character_handler<EncodingTraits>::handle(
            i, byte, current, end);
      }
    }
    ret.resize(i);
    return ret;
  }

  template <typename I, typename S>
  auto read_input_impl(I& current, S end, std::random_access_iterator_tag)
  {
    // TODO sanitize input
    auto const dist =
        std::min(static_cast<long int>(nb_bytes_to_read), end - current);

    auto const it = std::find_if(current, current + dist, [](auto byte) {
      return EncodingTraits::find_char(byte) == -1;
    });
    if (it != end)
    {
      // todo naming
      auto const real_dist = dist - (it - current);
      // FIXME redo how invalid chars are handled (with long inputs)
      detail::invalid_character_handler<EncodingTraits>::handle(
          it - current, *it, current, end);
      auto sp = detail::span<I>{current, current + real_dist};
      current += real_dist;
      return sp;
    }
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

    // auto const nb_loop_iterations =
    //     std::ldiv(input.size(), BitshiftTraits::nb_encoded_bytes);
    // std::bitset<BitshiftTraits::nb_total_decoded_bits> input_bits;
    //
    // for (auto i = 0; i < nb_loop_iterations.quot; ++i)
    // {
    //   auto const c =
    //       detail::encoded_input_reader<EncodingTraits>::read(_current, _end);
    //   auto const index = EncodingTraits::find_char(c);
    //
    //   if (index == -1)
    //   {
    //     detail::invalid_character_handler<EncodingTraits>::handle(
    //         i, c, _current, _end);
    //     break;
    //   }
    //
    //   std::bitset<BitshiftTraits::nb_total_decoded_bits> const
    //       decoded_byte_bits(index);
    //
    //   input_bits |=
    //       (decoded_byte_bits << (BitshiftTraits::nb_total_decoded_bits -
    //                              BitshiftTraits::nb_index_bits -
    //                              (BitshiftTraits::nb_index_bits * i)));
    // }
    // return {input_bits, (i * BitshiftTraits::nb_index_bits) / 8};
    // constexpr std::bitset<BitshiftTraits::nb_total_encoded_bits> mask(
    //     detail::pow<2, BitshiftTraits::BitshiftTraits::nb_index_bits>() - 1);
    //
    // auto const nb_loop_iterations =
    //     std::ldiv(input.size(), BitshiftTraits::nb_input_bytes);
    // output.resize(nb_loop_iterations.quot *
    // BitshiftTraits::BitshiftTraits::nb_decoded_bytes);
    //
    // for (auto i = 0u; i < nb_loop_iterations.quot; ++i)
    // {
    //   auto const input_bits = detail::input_to_bitset<BitshiftTraits>(
    //       input.begin() + (i * BitshiftTraits::nb_input_bytes),
    //       BitshiftTraits::nb_input_bytes);
    //
    //   detail::output_encoder<EncodingTraits>::encode(
    //       input_bits, output.begin() + (i *
    //       BitshiftTraits::BitshiftTraits::nb_decoded_bytes));
    // }
    //
    // if (nb_loop_iterations.rem)
    // {
    //   auto const nb_non_padded_bytes =
    //       ((8 * (nb_loop_iterations.rem)) /
    //       BitshiftTraits::BitshiftTraits::nb_index_bits) + 1;
    //   auto const nb_padded_bytes =
    //       BitshiftTraits::BitshiftTraits::nb_decoded_bytes -
    //       nb_non_padded_bytes;
    //
    //   auto const input_bits = detail::input_to_bitset<BitshiftTraits>(
    //       input.begin() +
    //           (nb_loop_iterations.quot * BitshiftTraits::nb_input_bytes),
    //       nb_loop_iterations.rem);
    //
    //   auto const old_size = output.size();
    //   output.resize(output.size() + nb_non_padded_bytes);
    //   detail::output_encoder<EncodingTraits>::encode(
    //       input_bits, output.begin() + old_size, nb_non_padded_bytes);
    //   detail::padding_writer<EncodingTraits>::write(std::back_inserter(output),
    //                                                 nb_padded_bytes);
    // }
  }

  struct read_result
  {
    std::bitset<BitshiftTraits::nb_total_decoded_bits> input_bits;
    std::size_t nb_read_bytes;
  };

  read_result read()
  {
    std::bitset<BitshiftTraits::nb_total_decoded_bits> input_bits;

    using std::begin;
    using std::end;

    auto i = 0;
    for (; i < BitshiftTraits::nb_encoded_bytes; ++i)
    {
      auto const c =
          detail::encoded_input_reader<EncodingTraits>::read(_current, _end);
      auto const index = EncodingTraits::find_char(c);

      if (index == -1)
      {
        detail::invalid_character_handler<EncodingTraits>::handle(
            i, c, _current, _end);
        break;
      }

      std::bitset<BitshiftTraits::nb_total_decoded_bits> const
          decoded_byte_bits(index);

      input_bits |=
          (decoded_byte_bits << (BitshiftTraits::nb_total_decoded_bits -
                                 BitshiftTraits::nb_index_bits -
                                 (BitshiftTraits::nb_index_bits * i)));
    }
    return {input_bits, (i * BitshiftTraits::nb_index_bits) / 8};
  }

  template <typename OutputIterator>
  void decode_input_bits(read_result const& res, OutputIterator out) const
  {
    std::bitset<BitshiftTraits::nb_total_decoded_bits> const mask{
        std::numeric_limits<std::uint64_t>::max()};

    for (int j = 0; j < res.nb_read_bytes; ++j)
    {
      auto const shift = ((BitshiftTraits::nb_decoded_bytes * 8) - 8 - (8 * j));
      auto const mask_bis =
          (mask >> (BitshiftTraits::nb_total_decoded_bits - 8)) << shift;
      auto const final_value = (res.input_bits & mask_bis) >> shift;
      auto const byte = static_cast<std::uint8_t>(final_value.to_ulong());
      *out++ = byte;
    }
  }

  Iterator _current{};
  Sentinel _end{};
};
}
}
}
}
