#pragma once

#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <string>

#include <boost/container/static_vector.hpp>

#include <mgs/detail/base_n/math.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename EncodingTraits,
          base_n_padding_policy = EncodingTraits::padding_policy>
struct encoded_input_reader
{
  template <typename Iterator, typename Sentinel>
  static char read(Iterator& current, Sentinel const sent)
  {
    if (current == sent)
    {
      throw unexpected_eof_error{EncodingTraits::encoding_name,
                                 "unexpected end of input"};
    }
    return *current++;
  }
};

template <typename EncodingTraits>
struct encoded_input_reader<EncodingTraits, base_n_padding_policy::optional>
{
  template <typename Iterator, typename Sentinel>
  static char read(Iterator& current, Sentinel const sent)
  {
    if (current == sent)
      return EncodingTraits::padding_character;
    return *current++;
  }
};

template <typename EncodingTraits,
          base_n_padding_policy = EncodingTraits::padding_policy>
struct non_alphabet_character_handler
{
private:
  static constexpr auto nb_output_bytes =
      decoded_bytes<sizeof(EncodingTraits::alphabet)>();

  static constexpr auto nb_input_bytes =
      encoded_bytes<sizeof(EncodingTraits::alphabet)>();

  static constexpr auto nb_output_bits = nb_output_bytes * 8;
  static constexpr auto nb_encoded_bits = nb_output_bits / nb_input_bytes;

  template <typename Iterator, typename Sentinel>
  static void expect_padding_bytes(Iterator& current, Sentinel const end, int n)
  {
    assert(current != end);
    assert(n > 0);

    for (; n > 0; --n)
    {
      auto const c = static_cast<char>(*current++);
      if (c != EncodingTraits::padding_character)
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

public:
  template <typename Iterator, typename Sentinel>
  static void handle(int i, char c, Iterator& current, Sentinel const sent)
  {
    if (c != EncodingTraits::padding_character)
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
      expect_padding_bytes(current, sent, nb_input_bytes - i - 1);
  }
};

template <typename EncodingTraits>
struct non_alphabet_character_handler<EncodingTraits, base_n_padding_policy::none>
{
  template <typename Iterator, typename Sentinel>
  static void handle(int, char c, Iterator&, Sentinel const)
  {
    using namespace std::string_literals;
    throw invalid_input_error{EncodingTraits::encoding_name,
                              "invalid encoded character: '"s + c + "'"};
  }
};

template <typename EncodingTraits>
class base_n_decoder
{
private:
  static constexpr auto nb_output_bytes =
      decoded_bytes<sizeof(EncodingTraits::alphabet)>();

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

      auto const c = encoded_input_reader<EncodingTraits>::read(current, sent);
      auto const index_it = EncodingTraits::find_char(c);

      if (index_it == alph_end)
      {
        non_alphabet_character_handler<EncodingTraits>::handle(
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
