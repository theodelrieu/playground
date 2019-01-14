#pragma once

#include <iosfwd>
#include <utility>

#include <mgs/adapters/concepts/iterable_transformed_input_adapter.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/concepts/codec_traits.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/static_asserts.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
template <typename CodecTraits>
class basic_codec
{
public:
  // TODO private
  template <typename Iterator,
            typename Sentinel = Iterator,
            typename = concepts::CodecTraits<CodecTraits, Iterator, Sentinel>>
  using encoder = decltype(CodecTraits::make_encoder(std::declval<Iterator>(),
                                                     std::declval<Sentinel>()));

  template <typename Iterator,
            typename Sentinel = Iterator,
            typename = concepts::CodecTraits<CodecTraits, Iterator, Sentinel>>
  using decoder = decltype(CodecTraits::make_decoder(std::declval<Iterator>(),
                                                     std::declval<Sentinel>()));

  using default_encoded_output = typename CodecTraits::default_encoded_output;
  using default_decoded_output = typename CodecTraits::default_decoded_output;

  template <typename Iterator, typename Sentinel>
  static adapters::concepts::TransformedInputAdapter<
      encoder<Iterator, Sentinel>>
  make_encoder(Iterator begin, Sentinel end)
  {
    return CodecTraits::make_encoder(std::move(begin), std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static adapters::concepts::TransformedInputAdapter<
      decoder<Iterator, Sentinel>>
  make_decoder(Iterator begin, Sentinel end)
  {
    return CodecTraits::make_decoder(std::move(begin), std::move(end));
  }

  template <typename T = default_encoded_output,
            typename Iterator = void,
            typename Sentinel = void,
            typename Encoder = adapters::concepts::TransformedInputAdapter<
                encoder<Iterator, Sentinel>>>
  static concepts::CodecOutput<T, Encoder> encode(Iterator it, Sentinel sent)
  {
    auto enc = CodecTraits::make_encoder(it, sent);
    return output_traits<T>::create(enc);
  }

  template <typename T = default_encoded_output,
            typename U = void,
            typename Iterable = meta::concepts::iterator::Iterable<U>,
            typename Iterator = meta::result_of_begin<Iterable const&>,
            typename Sentinel = meta::result_of_end<Iterable const&>,
            typename Encoder = adapters::concepts::TransformedInputAdapter<
                encoder<Iterator, Sentinel>>>
  static concepts::CodecOutput<T, Encoder> encode(U const& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::encode<T>(begin(it), end(it));
  }

  template <typename T = default_encoded_output,
            typename CharT = void,
            typename Traits = void,
            typename Encoder = adapters::concepts::TransformedInputAdapter<
                encoder<std::istreambuf_iterator<CharT, Traits>>>>
  static concepts::CodecOutput<T, Encoder> encode(
      std::basic_istream<CharT, Traits>& is)
  {
    return basic_codec::encode<T>(std::istreambuf_iterator<CharT, Traits>(is),
                                  std::istreambuf_iterator<CharT, Traits>());
  }

  template <typename T = default_decoded_output,
            typename Iterator = void,
            typename Sentinel = void,
            typename Decoder = adapters::concepts::TransformedInputAdapter<
                decoder<Iterator, Sentinel>>>
  static concepts::CodecOutput<T, Decoder> decode(Iterator it, Sentinel sent)
  {
    auto dec = CodecTraits::make_decoder(std::move(it), std::move(sent));
    return output_traits<T>::create(dec);
  }

  template <typename T = default_decoded_output,
            typename U = void,
            typename Iterable = meta::concepts::iterator::Iterable<U>,
            typename Iterator = meta::result_of_begin<Iterable const&>,
            typename Sentinel = meta::result_of_end<Iterable const&>,
            typename Decoder = adapters::concepts::TransformedInputAdapter<
                decoder<Iterator, Sentinel>>>
  static concepts::CodecOutput<T, Decoder> decode(U const& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::decode<T>(begin(it), end(it));
  }

  template <typename T = default_decoded_output,
            typename CharT = void,
            typename Traits = void,
            typename decoder = adapters::concepts::TransformedInputAdapter<
                decoder<std::istreambuf_iterator<CharT, Traits>>>>
  static concepts::CodecOutput<T, decoder> decode(
      std::basic_istream<CharT, Traits>& is)
  {
    return basic_codec::decode<T>(std::istreambuf_iterator<CharT, Traits>(is),
                                  std::istreambuf_iterator<CharT, Traits>());
  }

  template <typename T = default_encoded_output, std::size_t N = 0>
  static codecs::concepts::CodecOutput<T, encoder<char const*>> encode(
      char const (&tab)[N])
  {
    auto const begin = std::begin(tab);
    auto end = std::end(tab);
    if (tab[N - 1] == '\0')
      --end;
    return encode<T>(begin, end);
  }

  template <typename T = default_decoded_output, std::size_t N = 0>
  static codecs::concepts::CodecOutput<T, decoder<char const*>> decode(
      char const (&tab)[N])
  {
    auto const begin = std::begin(tab);
    auto end = std::end(tab);
    if (tab[N - 1] == '\0')
      --end;
    return decode<T>(begin, end);
  }
};
}
}
}
