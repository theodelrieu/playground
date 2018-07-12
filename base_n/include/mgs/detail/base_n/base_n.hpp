#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdlib>
#include <limits>
#include <type_traits>
#include <utility>

#include <mgs/detail/base_n/padding_policy.hpp>
#include <mgs/detail/iterators/adaptive_iterator.hpp>
#include <mgs/detail/meta/concepts/byte_integral.hpp>
#include <mgs/detail/meta/concepts/input_iterator.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits>
class base_n_encode
{
private:
  static constexpr auto nb_input_bits = EncodingTraits::nb_input_bytes * 8;
  static constexpr auto nb_encoded_bits =
      nb_input_bits / EncodingTraits::nb_output_bytes;

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
    for (; i < EncodingTraits::nb_input_bytes; ++i)
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

    if (EncodingTraits::padding_policy == base_n_padding_policy::required)
    {
      auto const nb_padding_bytes =
          EncodingTraits::nb_output_bytes - res.nb_non_padded_bytes;

      for (auto j = 0; j < nb_padding_bytes; ++j)
        *out++ = '=';
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
    encode_input_bits(res, out);
  }
};

template <typename EncodingTraits>
class base_n_decode
{
private:
  static constexpr auto nb_input_bits = EncodingTraits::nb_output_bytes * 8;
  static constexpr auto nb_encoded_bits =
      nb_input_bits / EncodingTraits::nb_input_bytes;

  struct read_result
  {
    std::bitset<nb_input_bits> input_bits;
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
      if (EncodingTraits::padding_policy == base_n_padding_policy::required)
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
    std::bitset<nb_input_bits> input_bits;

    using std::begin;
    using std::end;

    auto i = 0;
    for (; i < EncodingTraits::nb_input_bytes; ++i)
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
          expect_padding_bytes(
              current, sent, EncodingTraits::nb_input_bytes - i - 1);
        }
        break;
      }

      std::bitset<nb_input_bits> const decoded_byte_bits(
          std::distance(alph_begin, index_it));

      input_bits |= (decoded_byte_bits << (nb_input_bits - nb_encoded_bits -
                                           (nb_encoded_bits * i)));
    }
    return {input_bits, (i * nb_encoded_bits) / 8};
  }

  template <typename OutputIterator>
  void decode_input_bits(read_result const& res, OutputIterator& out) const
  {
    std::bitset<nb_input_bits> const mask{
        std::numeric_limits<std::uint64_t>::max()};

    for (int j = 0; j < res.nb_read_bytes; ++j)
    {
      auto const shift = (nb_input_bits - 8 - (8 * j));
      auto const mask_bis = (mask >> (nb_input_bits - 8)) << shift;
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

// TODO sfinae on encoding traits
template <typename EncodingTraits,
          typename UnderlyingIterator,
          typename Sentinel = UnderlyingIterator,
          typename = std::enable_if_t<
              detail::is_input_iterator<UnderlyingIterator>::value &&
              detail::is_sentinel<Sentinel, UnderlyingIterator>::value &&
              detail::is_byte_integral<detail::value_type_t<
                  std::iterator_traits<UnderlyingIterator>>>::value>>
class base_n_transformer
{
  using iterator =
      adaptive_iterator<base_n_transformer, std::input_iterator_tag>;

public:
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

  using difference_type = typename EncodingTraits::difference_type;
  using value_type = typename EncodingTraits::value_type;

  base_n_transformer() = default;
  base_n_transformer(UnderlyingIterator const& begin, Sentinel const& end);

  value_type get() const;
  void seek_forward(difference_type n);

  iterator begin() const;
  iterator end() const;

private:
  UnderlyingIterator _current{};
  Sentinel _end{};
  std::array<value_type, EncodingTraits::nb_output_bytes> _output;
  // only useful for decoding, but it allows to have the same class for both
  char _max_index{EncodingTraits::nb_output_bytes};
  char _index{_max_index};

  void _process_input();

  template <typename T, typename U, typename V, typename W>
  friend bool operator==(base_n_transformer<T, U, V, W> const& lhs,
                         base_n_transformer<T, U, V, W> const& rhs);
};
}
}

#include <mgs/detail/base_n/base_n_impl.hpp>
