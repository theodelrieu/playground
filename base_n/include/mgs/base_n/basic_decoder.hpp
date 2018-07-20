#pragma once

#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <limits>

#include <boost/container/static_vector.hpp>

#include <mgs/base_n/detail/encoded_input_reader.hpp>
#include <mgs/base_n/detail/invalid_character_handler.hpp>
#include <mgs/base_n/detail/math.hpp>
#include <mgs/base_n/padding_policy.hpp>

namespace mgs
{
namespace base_n
{
inline namespace v1
{
template <typename EncodingTraits>
class basic_decoder
{
private:
  static constexpr auto nb_output_bytes =
      detail::decoded_bytes<sizeof(EncodingTraits::alphabet)>();

  static constexpr auto nb_input_bytes =
      detail::encoded_bytes<sizeof(EncodingTraits::alphabet)>();

  static constexpr auto nb_output_bits = nb_output_bytes * 8;
  static constexpr auto nb_encoded_bits = nb_output_bits / nb_input_bytes;

  struct read_result
  {
    std::bitset<nb_output_bits> input_bits;
    int nb_read_bytes;
  };

  template <typename Iterator, typename Sentinel>
  read_result read(Iterator& current, Sentinel const sent) const
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
          detail::encoded_input_reader<EncodingTraits>::read(current, sent);
      auto const index_it = EncodingTraits::find_char(c);

      if (index_it == alph_end)
      {
        detail::invalid_character_handler<EncodingTraits>::handle(
            i, c, current, sent);
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
      auto const shift = (nb_output_bits - 8 - (8 * j));
      auto const mask_bis = (mask >> (nb_output_bits - 8)) << shift;
      auto const final_value = (res.input_bits & mask_bis) >> shift;
      auto const byte = static_cast<std::uint8_t>(final_value.to_ulong());
      *out++ = byte;
    }
  }

public:
  using value_type =
      boost::container::static_vector<std::uint8_t, nb_output_bytes>;

  template <typename Iterator, typename Sentinel>
  value_type operator()(Iterator& current, Sentinel const end) const
  {
    assert(current != end);

    auto const res = read(current, end);

    value_type ret;
    decode_input_bits(res, std::back_inserter(ret));
    return ret;
  }
};
}
}
}
