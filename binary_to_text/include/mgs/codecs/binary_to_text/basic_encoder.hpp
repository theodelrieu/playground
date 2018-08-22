#pragma once

#include <array>
#include <bitset>
#include <cstdint>
#include <limits>
#include <utility>

#include <mgs/codecs/binary_to_text/concepts/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>
#include <mgs/codecs/binary_to_text/detail/padding_writer.hpp>
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

  static constexpr auto nb_output_bytes = EncodingTraits::nb_output_bytes;
  static constexpr auto nb_input_bytes = EncodingTraits::nb_input_bytes;

  static constexpr auto nb_total_input_bits =
      detail::round_to_multiple_of<nb_input_bytes * 8, nb_output_bytes>();
  static constexpr auto nb_extra_input_bits = nb_total_input_bits % 8;
  static constexpr auto nb_encoded_bits = nb_total_input_bits / nb_output_bytes;

  static_assert(nb_total_input_bits % nb_output_bytes == 0,
                "The impossible has occurred");
  static_assert(detail::pow<2, nb_encoded_bits>() ==
                    sizeof(EncodingTraits::alphabet),
                "Alphabet size must be a power of 2");

public:
  using value_type = detail::static_vector<char, nb_output_bytes>;
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;

  basic_encoder() = default;

  basic_encoder(Iterator begin, Sentinel end)
    : _current(std::move(begin)), _end(std::move(end))
  {
  }

  value_type operator()()
  {
    if (_current == _end)
      return {};
    auto const res = read();
    value_type ret;
    encode_input_bits(res, std::back_inserter(ret));
    return ret;
  }

private:
  struct read_result
  {
    std::bitset<nb_total_input_bits> input_bits;
    std::size_t nb_non_padded_bytes;
  };

  read_result read()
  {
    std::bitset<nb_total_input_bits> input_bits;

    int i = 0;
    for (; i < nb_input_bytes; ++i)
    {
      if (_current == _end)
        break;
      auto byte = static_cast<std::uint8_t>(*_current++);
      // shifting on an integer type is a bit dangerous...
      // use bitset instead
      std::bitset<nb_total_input_bits> const byte_bits(byte);
      input_bits |= (byte_bits << (nb_total_input_bits - nb_extra_input_bits -
                                   8 - (8 * i)));
    }
    auto const nb_bits_read = (i * 8) + nb_extra_input_bits;
    auto const nb_non_padded_bytes = (nb_bits_read / nb_encoded_bits) +
                                     int((nb_bits_read % nb_encoded_bits) != 0);
    return {std::move(input_bits), nb_non_padded_bytes};
  }

  template <typename OutputIterator>
  void encode_input_bits(read_result const& res, OutputIterator out) const
  {
    std::bitset<nb_total_input_bits> const mask{
        std::numeric_limits<std::uint64_t>::max()};

    for (auto j = 0; j < res.nb_non_padded_bytes; ++j)
    {
      auto const shift =
          (nb_total_input_bits - nb_encoded_bits - (nb_encoded_bits * j));
      auto const mask_bis =
          ((mask >> (nb_total_input_bits - nb_encoded_bits)) << shift);
      auto const final_value = (res.input_bits & mask_bis) >> shift;
      auto const index = static_cast<std::uint8_t>(final_value.to_ulong());
      *out++ = EncodingTraits::alphabet[index];
    }

    detail::padding_writer<EncodingTraits>::write(
        out, nb_output_bytes - res.nb_non_padded_bytes);
  }

  Iterator _current{};
  Sentinel _end{};
};
}
}
}
}
