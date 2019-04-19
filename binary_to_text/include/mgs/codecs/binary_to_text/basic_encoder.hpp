#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <limits>
#include <utility>

#include <mgs/codecs/binary_to_text/concepts/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/bitset_utils.hpp>
#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/encoded_size.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>
#include <mgs/codecs/binary_to_text/detail/output_encoder.hpp>
#include <mgs/codecs/binary_to_text/detail/padding_writer.hpp>
#include <mgs/codecs/binary_to_text/detail/span.hpp>
#include <mgs/codecs/binary_to_text/detail/static_vector.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/concepts/sized_sentinel.hpp>
#include <mgs/meta/iter_concept.hpp>
#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace binary_to_text
{
template <typename Iterator, typename Sentinel, typename EncodingTraits>
class basic_encoder
{
private:
  static_assert(meta::concepts::is_input_iterator<Iterator>::value,
                "Iterator must be an InputIterator");
  static_assert(meta::concepts::is_sentinel<Sentinel, Iterator>::value,
                "Sentinel must be an Sentinel<Iterator>");
  static_assert(concepts::is_encoding_traits<EncodingTraits>::value,
                "Invalid encoding traits");

  static_assert(EncodingTraits::padding_policy != padding_policy::optional,
                "optional padding does not make sense when encoding");

  using BitshiftTraits = detail::bitshift_traits<EncodingTraits>;

  static_assert(BitshiftTraits::nb_encoded_bytes < 256,
                "nb_encoded_bytes must be lower than 256");

  static constexpr auto nb_bytes_to_read =
      (256 / BitshiftTraits::nb_encoded_bytes) *
      BitshiftTraits::nb_decoded_bytes;
  static_assert(nb_bytes_to_read % BitshiftTraits::nb_decoded_bytes == 0, "");

public:
  using buffer_type = detail::static_vector<char, 256>;
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;
  using encoding_traits = EncodingTraits;

  basic_encoder() = default;

  basic_encoder(Iterator begin, Sentinel end)
    : _current(std::move(begin)), _end(std::move(end))
  {
  }

  void operator()(buffer_type& out)
  {
    if (_current != this->_end)
      read_input(out);
    else
      out.resize(0);
  }

  template <typename I = Iterator,
            typename S = Sentinel,
            typename = meta::SizedSentinel<S, I>>
  meta::ssize_t max_transformed_size() const
  {
    return detail::encoded_size<EncodingTraits>{}(_end - _current);
  }

  underlying_iterator const& get_iterator() const
  {
    return _current;
  }

  underlying_sentinel const& get_sentinel() const
  {
    return _end;
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
      ret.push_back(static_cast<unsigned char>(*current++));
    }
    return ret;
  }

  template <typename I, typename S>
  auto read_input_impl(I& current, S end, std::random_access_iterator_tag)
  {
    auto const dist =
        std::min<meta::iter_difference_t<I>>(nb_bytes_to_read, end - current);

    detail::span<I> s{current, current + dist};
    current += dist;
    return s;
  }

  // TODO better name
  void read_input(buffer_type& output)
  {
    auto const input =
        read_input_impl(_current, _end, meta::iter_concept<Iterator>{});

    constexpr std::bitset<BitshiftTraits::nb_decoded_bits> mask(
        detail::pow<2, BitshiftTraits::nb_index_bits>() - 1);

    auto const nb_loop_iterations =
        std::lldiv(input.size(), BitshiftTraits::nb_decoded_bytes);
    output.resize(nb_loop_iterations.quot * BitshiftTraits::nb_encoded_bytes);

    for (auto i = 0; i < nb_loop_iterations.quot; ++i)
    {
      auto const input_bits = detail::decoded_to_bitset<BitshiftTraits>(
          input.begin() + (i * BitshiftTraits::nb_decoded_bytes),
          BitshiftTraits::nb_decoded_bytes);

      detail::output_encoder<EncodingTraits>::encode(
          input_bits, output.begin() + (i * BitshiftTraits::nb_encoded_bytes));
    }

    if (nb_loop_iterations.rem)
    {
      auto const nb_non_padded_bytes =
          ((8 * (nb_loop_iterations.rem)) / BitshiftTraits::nb_index_bits) +
          1;
      auto const nb_padded_bytes =
          BitshiftTraits::nb_encoded_bytes - nb_non_padded_bytes;

      auto const input_bits = detail::decoded_to_bitset<BitshiftTraits>(
          input.begin() +
              (nb_loop_iterations.quot * BitshiftTraits::nb_decoded_bytes),
          nb_loop_iterations.rem);

      auto const old_size = output.size();
      output.resize(output.size() + nb_non_padded_bytes);
      detail::output_encoder<EncodingTraits>::encode(
          input_bits, output.begin() + old_size, nb_non_padded_bytes);
      detail::padding_writer<EncodingTraits>::write(std::back_inserter(output),
                                                    nb_padded_bytes);
    }
  }

  Iterator _current{};
  Sentinel _end{};
};
}
}
}
}
