#pragma once

#include <bitset>
#include <cstdint>
#include <limits>
#include <utility>

#include <mgs/detail/base_n/math.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename EncodingTraits,
          base_n_padding_policy Policy = EncodingTraits::padding_policy>
struct padding_writer
{
  template <typename OutputIterator>
  static void write(OutputIterator&, int)
  {
  }
};

template <typename EncodingTraits>
struct padding_writer<EncodingTraits, base_n_padding_policy::required>
{
  template <typename OutputIterator>
  static void write(OutputIterator& out, int n)
  {
    while (n-- > 0)
      *out++ = EncodingTraits::padding_character;
  }
};

template <typename EncodingTraits>
class base_n_encoder
{
  static_assert(EncodingTraits::padding_policy !=
                    base_n_padding_policy::optional,
                "optional padding does not make sense when encoding");

public:
  // needed by transformer, useless once we use static_vector
  static constexpr auto nb_output_bytes =
      encoded_bytes<sizeof(EncodingTraits::alphabet)>();

private:
  static constexpr auto nb_input_bytes =
      decoded_bytes<sizeof(EncodingTraits::alphabet)>();

  static constexpr auto nb_input_bits = nb_input_bytes * 8;
  static constexpr auto nb_encoded_bits = nb_input_bits / nb_output_bytes;

  struct read_result
  {
    std::bitset<nb_input_bits> input_bits;
    int nb_non_padded_bytes;
  };

  template <typename Iterator, typename Sentinel>
  read_result read(Iterator& current, Sentinel const end) const
  {
    std::bitset<nb_input_bits> input_bits;

    int i = 0;
    for (; i < nb_input_bytes; ++i)
    {
      if (current == end)
        break;
      auto byte = static_cast<std::uint8_t>(*current++);
      // shifting on an integer type is a bit dangerous...
      // use bitset instead
      std::bitset<nb_input_bits> const byte_bits(byte);
      input_bits |= (byte_bits << (nb_input_bits - 8 - (8 * i)));
    }
    auto const nb_bits_read = i * 8;
    auto const nb_non_padded_bytes = (nb_bits_read / nb_encoded_bits) +
                                     int((nb_bits_read % nb_encoded_bits) != 0);
    return {std::move(input_bits), nb_non_padded_bytes};
  }

  template <typename OutputIterator>
  void encode_input_bits(read_result const& res, OutputIterator& out) const
  {
    std::bitset<nb_input_bits> const mask{
        std::numeric_limits<std::uint64_t>::max()};

    for (auto j = 0; j < res.nb_non_padded_bytes; ++j)
    {
      auto const shift =
          (nb_input_bits - nb_encoded_bits - (nb_encoded_bits * j));
      auto const mask_bis =
          ((mask >> (nb_input_bits - nb_encoded_bits)) << shift);
      auto const final_value = (res.input_bits & mask_bis) >> shift;
      auto const index = static_cast<std::uint8_t>(final_value.to_ulong());
      *out++ = EncodingTraits::alphabet[index];
    }

    padding_writer<EncodingTraits>::write(
        out, nb_output_bytes - res.nb_non_padded_bytes);
  }

public:
  template <typename Iterator, typename Sentinel, typename OutputIterator>
  void operator()(Iterator& current,
                  Sentinel const end,
                  OutputIterator& out) const
  {
    assert(current != end);

    auto const res = read(current, end);
    encode_input_bits(res, out);
  }
};
}
}
}