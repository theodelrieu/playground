#pragma once

#include <mgs/codecs/basic_codec.hpp>

#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>
#include <mgs/codecs/binary_to_text/detail/encoded_size.hpp>
#include <mgs/codecs/binary_to_text/detail/max_decoded_size.hpp>

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
  using base::encode;
  using base::decode;
  using base::make_encoder;
  using base::make_decoder;

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
    // TODO find the correct place to calculate size (i.e. where to retrieve
    // encoding traits...)
    using EncodingTraits =
        typename base::template encoder<char*,
                                        char*>::transformer::encoding_traits;

    return detail::encoded_size<EncodingTraits>{}(decoded_size);
  }

  static constexpr std::size_t max_decoded_size(std::size_t encoded_size)
  {
    using EncodingTraits =
        typename base::template decoder<char*,
                                        char*>::transformer::encoding_traits;

    return detail::max_decoded_size<EncodingTraits>{}(encoded_size);
  }
};
}
}
}
}
