#pragma once

#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <string>

#include <mgs/detail/base_n/math.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits>
class base_n_decoder
{
public:
  // needed by transformer, useless once we use static_vector
  static constexpr auto nb_output_bytes =
      decoded_bytes<sizeof(EncodingTraits::alphabet)>();

private:
  static constexpr auto nb_input_bytes =
      encoded_bytes<sizeof(EncodingTraits::alphabet)>();

  static constexpr auto nb_output_bits = nb_output_bytes * 8;
  static constexpr auto nb_encoded_bits = nb_output_bits / nb_input_bytes;

  struct read_result
  {
    std::bitset<nb_output_bits> input_bits;
    int nb_read_bytes;
  };

  template <typename Iterator, typename Sentinel>
  void expect_padding_bytes(Iterator& current, Sentinel const end, int n) const
  {
    assert(current != end);
    assert(n > 0);

    for (; n > 0; --n)
    {
      auto const c = static_cast<char>(*current++);
      if (c != '=')
      {
        using namespace std::string_literals;
        throw invalid_input_error{EncodingTraits::encoding_name,
                                  "invalid encoded character: '"s + c + "'"};
      }
      if (current == end && n != 1)
      {
        throw unexpected_eof_error{EncodingTraits::encoding_name,
                                   "unexpected end of encoded input"};
      }
    }
    if (current != end)
    {
      throw invalid_input_error{EncodingTraits::encoding_name,
                                "invalid encoded input"};
    }
  }

  template <typename Iterator, typename Sentinel>
  char read_encoded_char(Iterator& current, Sentinel const sent) const
  {
    if (current == sent)
    {
      if (EncodingTraits::padding_policy != base_n_padding_policy::optional)
      {
        throw unexpected_eof_error{EncodingTraits::encoding_name,
                                   "unexpected end of input"};
      }
      return '=';
    }
    return *current++;
  }

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

      auto const c = read_encoded_char(current, sent);
      auto const index_it = std::find(alph_begin, alph_end, c);

      if (index_it == alph_end)
      {
        if (c != '=')
        {
          using namespace std::string_literals;
          throw invalid_input_error{EncodingTraits::encoding_name,
                                    "invalid encoded character: '"s + c + "'"};
        }
        // find out if padding character is at a correct position
        auto const res = std::div(i * nb_encoded_bits, 8);
        if (res.quot == 0 || res.rem >= nb_encoded_bits)
        {
          throw invalid_input_error{EncodingTraits::encoding_name,
                                    "invalid encoded input"};
        }
        if (current != sent)
        {
          expect_padding_bytes(current, sent, nb_input_bytes - i - 1);
        }
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
  void decode_input_bits(read_result const& res, OutputIterator& out) const
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
  template <typename Iterator, typename Sentinel, typename OutputIterator>
  void operator()(Iterator& current,
                  Sentinel const end,
                  OutputIterator& out) const
  {
    assert(current != end);

    auto const res = read(current, end);
    decode_input_bits(res, out);
  }
};
}
}
