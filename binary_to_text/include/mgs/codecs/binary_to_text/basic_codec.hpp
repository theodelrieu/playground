#pragma once

#include <mgs/codecs/basic_codec.hpp>

#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace binary_to_text
{
template <typename CodecTraits>
class basic_codec : public codecs::basic_codec<CodecTraits>
{
  using base = codecs::basic_codec<CodecTraits>;

public:
  template <typename T = typename base::default_encoded_output,
            std::size_t N = 0>
  static auto encode(char const (&tab)[N])
      -> decltype(base::template encode<T>(std::declval<char const*>(),
                                           std::declval<char const*>()))
  {
    auto const end_it = std::find(std::begin(tab), std::end(tab), '\0');
    return base::template encode<T>(std::begin(tab), end_it);
  }

  template <typename T = typename base::default_decoded_output,
            std::size_t N = 0>
  static auto decode(char const (&tab)[N])
      -> decltype(base::template decode<T>(std::declval<char const*>(),
                                           std::declval<char const*>()))
  {
    auto const end_it = std::find(std::begin(tab), std::end(tab), '\0');
    return base::template decode<T>(std::begin(tab), end_it);
  }

  static constexpr std::size_t encoded_size(std::size_t decoded_size)
  {
    // TODO find the correct place to calculate size (i.e. where to retrieve encoding traits...)
    using EncodingTraits =
        typename base::template encoder<char*,
                                        char*>::transformer::encoding_traits;
    using BitshiftTraits = detail::bitshift_traits<EncodingTraits>;

    // FIXME only works for base2, padding policies
    return ((decoded_size / BitshiftTraits::nb_decoded_bytes) *
            BitshiftTraits::nb_encoded_bytes);
  }

  static constexpr std::size_t max_decoded_size(std::size_t encoded_size)
  {
    using EncodingTraits =
        typename base::template decoder<char*,
                                        char*>::transformer::encoding_traits;
    using BitshiftTraits = detail::bitshift_traits<EncodingTraits>;

    // FIXME only works for base2;
    return encoded_size % BitshiftTraits::nb_encoded_bytes != 0 ?
               0 :
               encoded_size / BitshiftTraits::nb_encoded_bytes;
  }
};
}
}
}
}
