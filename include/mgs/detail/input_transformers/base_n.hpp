#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdlib>
#include <type_traits>

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
public:
  template <typename Iterator, typename Sentinel, typename OutputIterator>
  void operator()(Iterator& current,
                  Sentinel const end,
                  OutputIterator& out) const
  {
    assert(current != end);

    constexpr auto total_bits = EncodingTraits::nb_input_bytes * 8;
    constexpr auto encoded_bits = total_bits / EncodingTraits::nb_output_bytes;

    std::bitset<total_bits> bits;
    auto mask = bits;
    mask.flip();

    int i = 0;
    for (; i < EncodingTraits::nb_input_bytes; ++i)
    {
      if (current == end)
        break;
      auto byte = static_cast<std::uint8_t>(*current++);
      bits |= (byte << (total_bits - 8 - (8 * i)));
    }

    for (auto j = 0; j < i + 1; ++j)
    {
      auto shift = (total_bits - encoded_bits - (encoded_bits * j));
      auto mask_bis = ((mask >> (total_bits - encoded_bits)) << shift);
      auto l = bits & mask_bis;
      l >>= shift;
      auto index = static_cast<std::uint8_t>(l.to_ulong());
      *out++ = EncodingTraits::alphabet[index];
    }

    auto const nb_bits_read = 8 * i;
    auto const nb_non_padded_bytes =
        (nb_bits_read / encoded_bits) + int((nb_bits_read % encoded_bits) != 0);
    auto const nb_padding_bytes =
        EncodingTraits::nb_output_bytes - nb_non_padded_bytes;

    for (auto j = 0; j < nb_padding_bytes; ++j)
      *out++ = '=';
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
