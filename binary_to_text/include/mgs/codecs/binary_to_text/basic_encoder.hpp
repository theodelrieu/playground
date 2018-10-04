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
  // WIP
template <typename T>
class span
{
public:
  span(T begin, T const end) : begin(begin), end(end)
  {
  }
  auto& operator[](std::size_t n)
  {
    return begin[n];
  }

  std::size_t size() const
  {
    return end - begin;
  }

  auto const& operator[](std::size_t n) const
  {
    return begin[n];
  }
  private:
    T begin;
    T end;

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

public:
  // TODO assert 256 < nb_input_bytes or smth like that
  using value_type = detail::static_vector<char, 256>;
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
    return read_input();
  }

protected:
  struct read_result
  {
    std::bitset<nb_total_input_bits> input_bits;
    std::size_t nb_non_padded_bytes;
  };

  template <typename I, typename S>
  value_type read_input_impl(I& current, S end, std::input_iterator_tag)
  {
    value_type ret;
    auto i = 0;
    for (; i < 192; ++i)
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
    // 192 because max encoded output is 256
    auto dist = std::min(192l, end - current);

    detail::span<I> s{current, current + dist};
    current += dist;
    return s;
  }

  value_type read_input()
  {
    auto const input = read_input_impl(
        _current,
        _end,
        typename std::iterator_traits<Iterator>::iterator_category{});
    auto const nb_loop_iterations = std::div(input.size(), 6);

    value_type ret;
    ret.resize(nb_loop_iterations.quot * 8);
    // FIXME make it work for other than base64
    auto i = 0, j = 0;
    for (; i < nb_loop_iterations.quot * 6; i += 6, j += 8)
    {
      std::bitset<48> bits;
      bits |= ((std::bitset<48>(input[i])) << 48 - 8);
      bits |= ((std::bitset<48>(input[i + 1])) << 48 - 16);
      bits |= ((std::bitset<48>(input[i + 2])) << 48 - 24);
      bits |= ((std::bitset<48>(input[i + 3])) << 48 - 32);
      bits |= ((std::bitset<48>(input[i + 4])) << 48 - 40);
      bits |= ((std::bitset<48>(input[i + 5])));

      std::bitset<48> mask(0x3F);

      ret[j] = EncodingTraits::alphabet[((bits >> 42) & mask).to_ulong()];
      ret[j + 1] = EncodingTraits::alphabet[((bits >> 36) & mask).to_ulong()];
      ret[j + 2] = EncodingTraits::alphabet[((bits >> 30) & mask).to_ulong()];
      ret[j + 3] = EncodingTraits::alphabet[((bits >> 24) & mask).to_ulong()];
      ret[j + 4] = EncodingTraits::alphabet[((bits >> 18) & mask).to_ulong()];
      ret[j + 5] = EncodingTraits::alphabet[((bits >> 12) & mask).to_ulong()];
      ret[j + 6] = EncodingTraits::alphabet[((bits >> 6) & mask).to_ulong()];
      ret[j + 7] = EncodingTraits::alphabet[(bits & mask).to_ulong()];
    }
    if (nb_loop_iterations.rem){
    std::bitset<48> bits;

    for (auto k = 0; k < nb_loop_iterations.rem; ++k)
    {
      bits |= ((std::bitset<48>(input[i + k])) << (40 - (8 * k)));
    }
    auto const nb_non_padded_bytes = ((nb_loop_iterations.rem * 8 )/ nb_encoded_bits) +
                                     int(((nb_loop_iterations.rem * 8) % nb_encoded_bits) != 0);

  //     auto const shift =
  //         (nb_total_input_bits - nb_encoded_bits - (nb_encoded_bits * j));
  //     auto const mask_bis =
  //         ((mask >> (nb_total_input_bits - nb_encoded_bits)) << shift);
    for (auto k = 0; k < nb_non_padded_bytes; ++k)
    {
      ret.push_back(
          EncodingTraits::alphabet[((bits >> (42 - (6 * k))) & std::bitset<48>(0x3F))
                                       .to_ulong()]);
    }
    for (auto k = 0; k < nb_output_bytes - (nb_non_padded_bytes % nb_output_bytes); ++k)
      ret.push_back('=');
    }
    return ret;
    // FIXME padding
  }

  template <typename I, typename S, typename T>
  friend bool operator==(basic_encoder<I, S, T> const&,
                         basic_encoder<I, S, T> const&);

  // read_result read()
  // {
  //   std::bitset<nb_total_input_bits> input_bits;
  //
  //   int i = 0;
  //   for (; i < nb_input_bytes; ++i)
  //   {
  //     if (_current == _end)
  //       break;
  //     auto byte = static_cast<std::uint8_t>(*_current++);
  //     // shifting on an integer type is a bit dangerous...
  //     // use bitset instead
  //     std::bitset<nb_total_input_bits> const byte_bits(byte);
  //     input_bits |= (byte_bits << (nb_total_input_bits - nb_extra_input_bits -
  //                                  8 - (8 * i)));
  //   }
  //   auto const nb_bits_read = (i * 8) + nb_extra_input_bits;
  //   auto const nb_non_padded_bytes = (nb_bits_read / nb_encoded_bits) +
  //                                    int((nb_bits_read % nb_encoded_bits) != 0);
  //   return {std::move(input_bits), nb_non_padded_bytes};
  // }
  //
  // template <typename OutputIterator>
  // void encode_input_bits(read_result const& res, OutputIterator out) const
  // {
  //   std::bitset<nb_total_input_bits> const mask{
  //       std::numeric_limits<std::uint64_t>::max()};
  //
  //   for (auto j = 0; j < res.nb_non_padded_bytes; ++j)
  //   {
  //     auto const shift =
  //         (nb_total_input_bits - nb_encoded_bits - (nb_encoded_bits * j));
  //     auto const mask_bis =
  //         ((mask >> (nb_total_input_bits - nb_encoded_bits)) << shift);
  //     auto const final_value = (res.input_bits & mask_bis) >> shift;
  //     auto const index = static_cast<std::uint8_t>(final_value.to_ulong());
  //     *out++ = EncodingTraits::alphabet[index];
  //   }
  //
  //   detail::padding_writer<EncodingTraits>::write(
  //       out, nb_output_bytes - res.nb_non_padded_bytes);
  // }
  //
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
