#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <limits>
#include <utility>

#include <mgs/base_n/concepts/encoding_traits.hpp>
#include <mgs/base_n/detail/bitset_utils.hpp>
#include <mgs/base_n/detail/bitshift_traits.hpp>
#include <mgs/base_n/detail/encoded_size.hpp>
#include <mgs/base_n/detail/math.hpp>
#include <mgs/base_n/detail/output_encoder.hpp>
#include <mgs/base_n/detail/padding_writer.hpp>
#include <mgs/base_n/detail/span.hpp>
#include <mgs/base_n/detail/static_vector.hpp>
#include <mgs/base_n/padding_policy.hpp>
#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/concepts/sized_input_source.hpp>
#include <mgs/codecs/input_source_view.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace base_n
{
template <typename Traits, typename IS>
class encode_algorithm
{
private:
  static_assert(base_n::is_encoding_traits<Traits>::value,
                "Traits does not model base_n::encoding_traits");
  static_assert(codecs::is_input_source<IS, unsigned char*>::value,
                "IS does not model codecs::input_source<unsigned char*>");

  static_assert(Traits::padding_policy != padding_policy::optional,
                "optional padding does not make sense when encoding");

  using BitshiftTraits = detail::bitshift_traits<Traits>;

  static_assert(BitshiftTraits::nb_encoded_bytes < 256,
                "nb_encoded_bytes must be lower than 256");

  // FIXME meta::ssize_t?
  static constexpr std::size_t nb_bytes_to_read =
      (256 / BitshiftTraits::nb_encoded_bytes) *
      BitshiftTraits::nb_decoded_bytes;
  static_assert(nb_bytes_to_read % BitshiftTraits::nb_decoded_bytes == 0, "");

  using buffer_type = detail::static_vector<char, 256>;

public:
  using element_type = char;

  explicit encode_algorithm(IS is) : _input_source(std::move(is))
  {
  }

  template <typename T = IS, typename = codecs::sized_input_source<T>>
  meta::ssize_t max_remaining_size() const
  {
    return detail::encoded_size<Traits>{}(_input_source.max_remaining_size());
  }

  template <typename O>
  meta::ssize_t read(meta::output_iterator<O, element_type> o, meta::ssize_t n)
  {
    if (_buffer.size() == _index)
    {
      read_input_source();
      _index = 0;
    }
    auto const to_read = std::min<meta::ssize_t>(_buffer.size() - _index, n);
    std::copy_n(_buffer.data() + _index, to_read, o);
    _index += to_read;
    return to_read;
  }

private:
  auto read_input_source_impl()
  {
    detail::static_vector<unsigned char, nb_bytes_to_read> ret;
    ret.resize(nb_bytes_to_read);
    // FIXME helper method to read while not eof
    auto nb_read = _input_source.read(ret.data(), nb_bytes_to_read);
    ret.resize(nb_read);
    return ret;
  }

  void read_input_source()
  {
    auto const input = read_input_source_impl();

    constexpr std::bitset<BitshiftTraits::nb_decoded_bits> mask(
        detail::pow<2, BitshiftTraits::nb_index_bits>() - 1);

    auto const nb_loop_iterations =
        std::lldiv(input.size(), BitshiftTraits::nb_decoded_bytes);
    _buffer.resize(nb_loop_iterations.quot * BitshiftTraits::nb_encoded_bytes);

    for (auto i = 0; i < nb_loop_iterations.quot; ++i)
    {
      auto const input_bits = detail::decoded_to_bitset<BitshiftTraits>(
          input.begin() + (i * BitshiftTraits::nb_decoded_bytes),
          BitshiftTraits::nb_decoded_bytes);

      detail::output_encoder<Traits>::encode(
          input_bits, _buffer.begin() + (i * BitshiftTraits::nb_encoded_bytes));
    }

    if (nb_loop_iterations.rem)
    {
      auto const nb_non_padded_bytes =
          ((8 * (nb_loop_iterations.rem)) / BitshiftTraits::nb_index_bits) + 1;
      auto const nb_padded_bytes =
          BitshiftTraits::nb_encoded_bytes - nb_non_padded_bytes;

      auto const input_bits = detail::decoded_to_bitset<BitshiftTraits>(
          input.begin() +
              (nb_loop_iterations.quot * BitshiftTraits::nb_decoded_bytes),
          nb_loop_iterations.rem);

      auto const old_size = _buffer.size();
      _buffer.resize(_buffer.size() + nb_non_padded_bytes);
      detail::output_encoder<Traits>::encode(
          input_bits, _buffer.begin() + old_size, nb_non_padded_bytes);
      detail::padding_writer<Traits>::write(std::back_inserter(_buffer),
                                            nb_padded_bytes);
    }
  }

  IS _input_source;
  buffer_type _buffer;
  meta::ssize_t _index{};
};

template <typename Traits, typename IS>
constexpr std::size_t encode_algorithm<Traits, IS>::nb_bytes_to_read;
}
}
