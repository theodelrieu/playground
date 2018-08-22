#pragma once

#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <limits>

#include <mgs/codecs/binary_to_text/concepts/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/encoded_input_reader.hpp>
#include <mgs/codecs/binary_to_text/detail/invalid_character_handler.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>
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

  static constexpr auto nb_output_bytes = EncodingTraits::nb_output_bytes;
  static constexpr auto nb_input_bytes = EncodingTraits::nb_input_bytes;

  static constexpr auto nb_output_bits =
      detail::round_to_multiple_of<nb_output_bytes * 8, nb_input_bytes>();
  static constexpr auto nb_encoded_bits = nb_output_bits / nb_input_bytes;

  static_assert(nb_output_bits % nb_input_bytes == 0,
                "The impossible has occurred");
  static_assert(detail::pow<2, nb_encoded_bits>() ==
                    sizeof(EncodingTraits::alphabet),
                "Alphabet size must be a power of 2");

public:
  using value_type = detail::static_vector<std::uint8_t, nb_output_bytes>;
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;

  basic_decoder() = default;

  basic_decoder(Iterator begin, Sentinel end)
    : _current(std::move(begin)), _end(std::move(end))
  {
  }

  value_type operator()()
  {
    if (_current == _end)
      return {};
    auto const res = read();

    value_type ret;
    decode_input_bits(res, std::back_inserter(ret));
    return ret;
  }

  struct read_result
  {
    std::bitset<nb_output_bits> input_bits;
    std::size_t nb_read_bytes;
  };

  read_result read()
  {
    std::bitset<nb_output_bits> input_bits;

    using std::begin;
    using std::end;

    auto i = 0;
    for (; i < nb_input_bytes; ++i)
    {
      auto const alph_begin = begin(EncodingTraits::alphabet);
      auto const alph_end = end(EncodingTraits::alphabet);

      auto const c =
          detail::encoded_input_reader<EncodingTraits>::read(_current, _end);
      auto const index_it = EncodingTraits::find_char(c);

      if (index_it == alph_end)
      {
        detail::invalid_character_handler<EncodingTraits>::handle(
            i, c, _current, _end);
        break;
      }

      std::bitset<nb_output_bits> const decoded_byte_bits(
          std::distance(alph_begin, index_it));

      input_bits |= (decoded_byte_bits << (nb_output_bits - nb_encoded_bits -
                                           (nb_encoded_bits * i)));
    }
    return {input_bits, (i * nb_encoded_bits) / 8};
  }

  template <typename OutputIterator>
  void decode_input_bits(read_result const& res, OutputIterator out) const
  {
    std::bitset<nb_output_bits> const mask{
        std::numeric_limits<std::uint64_t>::max()};

    for (int j = 0; j < res.nb_read_bytes; ++j)
    {
      auto const shift = ((nb_output_bytes * 8) - 8 - (8 * j));
      auto const mask_bis = (mask >> (nb_output_bits - 8)) << shift;
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
