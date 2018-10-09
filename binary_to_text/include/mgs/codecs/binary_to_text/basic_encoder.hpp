#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <limits>
#include <utility>

#include <mgs/codecs/binary_to_text/concepts/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>
#include <mgs/codecs/binary_to_text/detail/padding_writer.hpp>
#include <mgs/codecs/binary_to_text/detail/static_vector.hpp>
#include <mgs/codecs/binary_to_text/detail/span.hpp>
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
namespace detail
{
  template <typename EncodingTraits,
          std::size_t InputBytes = EncodingTraits::nb_input_bytes,
          std::size_t OutputBytes = EncodingTraits::nb_output_bytes>
struct output_encoder
{
  static constexpr auto nb_input_bytes = InputBytes;
  static constexpr auto nb_output_bytes = OutputBytes;
  static constexpr auto nb_total_input_bits =
      detail::round_to_multiple_of<nb_input_bytes * 8, nb_output_bytes>();
  static constexpr auto nb_extra_input_bits = nb_total_input_bits % 8;
  static constexpr auto nb_encoded_bits = nb_total_input_bits / nb_output_bytes;
  static constexpr auto shift = nb_total_input_bits - nb_encoded_bits;

  template <std::size_t N, typename OutputIterator>
  static void encode(std::bitset<N> const& input_bits,
                     OutputIterator out,
                     std::bitset<N> const& mask)
  {
    for (auto i = 0u; i < nb_output_bytes; ++i)
    {
      auto const index = static_cast<std::uint8_t>(
          ((input_bits >> (shift - (i * nb_encoded_bits))) & mask).to_ulong());
      *(out + i) = EncodingTraits::alphabet[index];
    }
    //
    // out[0] = EncodingTraits::alphabet[((input_bits >> 18) & mask).to_ulong()];
    // out[1]= 
    //     EncodingTraits::alphabet[((input_bits >> 12) & mask).to_ulong()];
    // out[2] =
    //     EncodingTraits::alphabet[((input_bits >> 6) & mask).to_ulong()];
    // out[3] = EncodingTraits::alphabet[((input_bits & mask).to_ulong())];
  }
};



template <typename EncodingTraits>
struct find_good_name
{
  static constexpr auto nb_input_bytes = EncodingTraits::nb_input_bytes;
  static constexpr auto nb_output_bytes = EncodingTraits::nb_output_bytes;
  static constexpr auto nb_total_input_bits =
      detail::round_to_multiple_of<nb_input_bytes * 8, nb_output_bytes>();
  static constexpr auto nb_extra_input_bits = nb_total_input_bits % 8;
  static constexpr auto nb_encoded_bits = nb_total_input_bits / nb_output_bytes;
  static constexpr auto shift = nb_total_input_bits - nb_encoded_bits;

  template <typename T>
  static void read_input(T const& input, static_vector<char, 256>& output)
  {
    static std::bitset<nb_total_input_bits> const mask(pow<2, nb_encoded_bits>() - 1);

    auto const nb_loop_iterations = std::ldiv(input.size(), nb_input_bytes);

    output.resize(nb_loop_iterations.quot * nb_output_bytes);
    for (auto i = 0u; i < nb_loop_iterations.quot; ++i)
    {
      std::bitset<nb_total_input_bits> input_bits;

      for (auto j = 0u; j < nb_input_bytes; ++j)
      {
        input_bits |=
            (decltype(input_bits)(input[(i * nb_input_bytes) + j])
             << (nb_total_input_bits - nb_extra_input_bits - 8 - (8 * j)));
      }

      output_encoder<EncodingTraits>::encode(
          input_bits, output.begin() + (i * nb_output_bytes), mask);
    }

    if (nb_loop_iterations.rem)
    {
      auto const nb_non_padded_bytes =
          ((8 * (nb_loop_iterations.rem)) / nb_encoded_bits) + 1;
      auto const nb_padded_bytes = (nb_output_bytes - nb_non_padded_bytes);
      output.resize(output.size() + nb_padded_bytes + nb_non_padded_bytes);
      // FIXME handle no padding


      std::bitset<nb_total_input_bits> input_bits;

      for (auto i = 0u; i < nb_loop_iterations.rem; ++i)
      {
        input_bits |=
            (decltype(input_bits)(
                 input[nb_loop_iterations.quot * nb_input_bytes + i])
             << (nb_total_input_bits - nb_extra_input_bits - 8 - (8 * i)));
      }

      for (auto i = 0u; i < nb_loop_iterations.rem + 1; ++i)
      {
        auto const index = static_cast<std::uint8_t>(
            ((input_bits >> (shift - (i * nb_encoded_bits))) & mask)
                .to_ulong());
        output[nb_loop_iterations.quot * nb_output_bytes + i] =
            EncodingTraits::alphabet[index];
      }

      padding_writer<EncodingTraits>::write(output.end() - nb_padded_bytes,
                                            nb_padded_bytes);
    }
  }
};
}

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
  static_assert(nb_output_bytes % 2 == 0,
                "nb_output_bytes must be a power of two");
  static_assert(nb_output_bytes <= 8, "nb_output_bytes must be <= 8");

  static constexpr auto nb_bytes_to_read =
      (256 / nb_output_bytes) * nb_input_bytes;

  // TODO
  // nb_output_bytes must be a power of 2
  // (256 / nb_output_bytes) * nb_input_bytes = nb_input_bytes_to_read
  // assert nb_output_bytes <= 8
  // nb read_bytes per loop + nb bytes to encode per loop: *= 8 /
  // nb_output_bytes


public:
  using value_type = detail::static_vector<char, 256>;
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;

  basic_encoder() = default;

  basic_encoder(Iterator begin, Sentinel end)
    : _current(std::move(begin)), _end(std::move(end))
  {
  }

  void operator()(value_type& out)
  {
    if (_current != this->_end)
      read_input(out);
    else
      out.resize(0);
  }

private:
  template <typename I, typename S>
  auto read_input_impl(I& current, S end, std::input_iterator_tag)
  {
    detail::static_vector<unsigned char, nb_bytes_to_read> ret;
    auto i = 0;
    for (; i < nb_bytes_to_read; ++i)
    {
      if (current == end)
        break;
      ret.push_back(*current++);
    }
    return ret;
  }

  template <typename I, typename S>
  auto read_input_impl(I& current, S end, std::random_access_iterator_tag)
  {
    auto const dist = std::min(static_cast<long int>(nb_bytes_to_read), end - current);

    detail::span<I> s{current, current + dist};
    current += dist;
    return s;
  }

  void read_input(value_type& output)
  {
    auto const input = read_input_impl(
        _current,
        _end,
        typename std::iterator_traits<Iterator>::iterator_category{});

    detail::find_good_name<EncodingTraits>::read_input(input, output);
  }

  template <typename I, typename S, typename T>
  friend bool operator==(basic_encoder<I, S, T> const&,
                         basic_encoder<I, S, T> const&);

  Iterator _current{};
  Sentinel _end{};
};

template <typename I, typename S, typename T>
bool operator==(basic_encoder<I, S, T> const& lhs,
                basic_encoder<I, S, T> const& rhs)
{
  return (lhs._current == lhs._end || rhs._current == rhs._end) ?
             (lhs._current == lhs._end && rhs._current == rhs._end) :
             lhs._current == rhs._current;
}

template <typename I, typename S, typename T>
bool operator!=(basic_encoder<I, S, T> const& lhs,
                basic_encoder<I, S, T> const& rhs)
{
  return !(lhs == rhs);
}
}
}
}
}
