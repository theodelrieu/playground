#pragma once

#include <array>
#include <iostream>
#include <bitset>
#include <cstdint>
#include <limits>
#include <utility>

#include <boost/container/static_vector.hpp>

#include <mgs/base_n/detail/math.hpp>
#include <mgs/base_n/detail/padding_writer.hpp>
#include <mgs/base_n/padding_policy.hpp>

namespace mgs
{
namespace base_n
{
inline namespace v1
{
template <typename EncodingTraits>
class basic_encoder
{
  static_assert(EncodingTraits::padding_policy != padding_policy::optional,
                "optional padding does not make sense when encoding");

private:
  static constexpr auto nb_output_bytes = 3u;
      // detail::encoded_bytes<sizeof(EncodingTraits::alphabet)>();

  static constexpr auto nb_input_bytes = 1u;
      // detail::decoded_bytes<sizeof(EncodingTraits::alphabet)>();

  static constexpr auto nb_input_bits = nb_input_bytes * 8;
  static constexpr auto nb_encoded_bits = 3u;

  struct read_result
  {
    std::bitset<nb_input_bits> input_bits;
    std::size_t nb_non_padded_bytes;
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
  void encode_input_bits(read_result const& res, OutputIterator out) const
  {
    std::bitset<nb_input_bits> const mask{
        std::numeric_limits<std::uint64_t>::max()};

    std::cout << "input bits: " << res.input_bits.to_string() << std::endl;
    for (auto j = 0; j < res.nb_non_padded_bytes; ++j)
    {
      auto const shift =
          (nb_input_bits - nb_encoded_bits - (nb_encoded_bits * j) + 1);
      std::cout << "shifting by " << shift << std::endl;
      auto const mask_bis =
          ((mask >> (nb_input_bits - nb_encoded_bits)) << shift);
      auto const final_value = (res.input_bits & mask_bis) >> shift;
    std::cout << "shifted bits: " << final_value.to_string() << std::endl;
      auto const index = static_cast<std::uint8_t>(final_value.to_ulong());
    std::cout << "index: " << (int)index << std::endl;
      *out++ = EncodingTraits::alphabet[index];
    }

    detail::padding_writer<EncodingTraits>::write(
        out, nb_output_bytes - res.nb_non_padded_bytes);
  }

public:
  using value_type = boost::container::static_vector<char, nb_output_bytes>;

  template <typename Iterator, typename Sentinel>
  value_type operator()(Iterator& current, Sentinel const end) const
  {
    assert(current != end);

    auto const res = read(current, end);
    value_type ret;
    encode_input_bits(res, std::back_inserter(ret));
    return ret;
  }
};
}
}
}
