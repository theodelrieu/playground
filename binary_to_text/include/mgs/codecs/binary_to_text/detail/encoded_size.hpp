#pragma once

#include <cstddef>

#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>

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
          padding_policy = EncodingTraits::padding_policy>
struct encoded_size;

template <typename EncodingTraits>
struct encoded_size<EncodingTraits, padding_policy::required>
{
  constexpr std::size_t operator()(std::size_t decoded_size) const
  {
    using BitshiftTraits = bitshift_traits<EncodingTraits>;

    auto const quot = decoded_size / BitshiftTraits::nb_decoded_bytes;
    auto const rem = decoded_size % BitshiftTraits::nb_decoded_bytes;

    return quot * BitshiftTraits::nb_encoded_bytes +
           int(rem > 0) * BitshiftTraits::nb_encoded_bytes;
  }
};

template <typename EncodingTraits>
struct encoded_size<EncodingTraits, padding_policy::none>
{
  constexpr std::size_t operator()(std::size_t decoded_size) const
  {
    using BitshiftTraits = bitshift_traits<EncodingTraits>;

    auto const quot = (decoded_size * BitshiftTraits::nb_encoded_bytes) /
                      BitshiftTraits::nb_decoded_bytes;
    auto const rem = (decoded_size * BitshiftTraits::nb_encoded_bytes) %
                      BitshiftTraits::nb_decoded_bytes;

    return quot + int(rem > 0);
  }
};

// optional does not make sense when encoding.
template <typename EncodingTraits>
struct encoded_size<EncodingTraits, padding_policy::optional>;
}
}
}
}
}
