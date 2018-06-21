#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdlib>
#include <limits>
#include <type_traits>
#include <utility>

#include <mgs/detail/iterators/adaptive_iterator.hpp>
#include <mgs/detail/meta/concepts/byte_integral.hpp>
#include <mgs/detail/meta/concepts/input_iterator.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>
#include <mgs/exceptions/parse_error.hpp>

namespace mgs
{
namespace detail
{
template <int TotalBits, int EncodedBits>
int base_n_nb_padding_bytes(int nb_bits_read)
{
  auto const nb_non_padded_bytes =
      (nb_bits_read / EncodedBits) + int((nb_bits_read % EncodedBits) != 0);

  return (TotalBits / EncodedBits) - nb_non_padded_bytes;
}

// TODO where to put those??
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

    auto const nb_padding_bytes =
        EncodingTraits::nb_output_bytes - res.nb_non_padded_bytes;

    for (auto j = 0; j < nb_padding_bytes; ++j)
      *out++ = '=';
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
  template <typename Iterator, typename Sentinel>
  void expect_padding_bytes(Iterator& current, Sentinel const end, int n) const
  {
    assert(current != end);
    for (; n > 0; --n)
    {
      auto const c = *current++;
      if (c != '=')
        throw parse_error{"base64: unexpected padding character"};
      if (current == end && n != 1)
        throw parse_error{"base64: unexpected end of input"};
    }
    if (current != end)
      throw parse_error{"base64: unexpected padding character"};
  }

public:
  template <typename Iterator, typename Sentinel, typename OutputIterator>
  void operator()(Iterator& current,
                  Sentinel const end,
                  OutputIterator& out) const
  {
    assert(current != end);

    constexpr auto total_bits = EncodingTraits::nb_output_bytes * 8;
    constexpr auto encoded_bits = total_bits / EncodingTraits::nb_input_bytes;

    auto const alph_begin = std::begin(EncodingTraits::alphabet);
    auto const alph_end = std::end(EncodingTraits::alphabet);

    std::bitset<total_bits> bits;
    auto mask = bits;
    mask.flip();

    int i = 0;
    for (; i < EncodingTraits::nb_input_bytes; ++i)
    {
      if (current == end)
        throw parse_error{"base64: unexpected EOF"};
      auto c = static_cast<char>(*current);
      auto const index_it = std::find(alph_begin, alph_end, c);
      if (index_it == alph_end)
      {
        // true for base64 and base32, base16 does not have padding.
        auto const min_padding_position = 2;
        if (i < 2)
          throw parse_error{"base64: unexpected padding character"};
        expect_padding_bytes(current, end, EncodingTraits::nb_input_bytes - i);
        break;
      }
      bits |= (std::distance(alph_begin, index_it)
               << (total_bits - encoded_bits - (encoded_bits * i)));
      ++current;
    }

    for (int j = 0; j < i - 1; ++j)
    {
      auto shift = (total_bits - 8 - (8 * j));
      auto mask_bis = (mask >> (total_bits - 8)) << shift;
      auto l = bits & mask_bis;
      l >>= shift;
      auto byte = static_cast<std::uint8_t>(l.to_ulong());
      *out++ = byte;
    }
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

#include <mgs/detail/input_transformers/base_n_impl.hpp>
