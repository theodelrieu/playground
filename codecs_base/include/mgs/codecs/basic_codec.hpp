#pragma once

#include <utility>

#include <mgs/adapters/concepts/iterable_transformed_input_adapter.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected.hpp>

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
  template <typename Iterator,
            typename Sentinel = Iterator,
            typename = meta::concepts::iterator::Sentinel<Sentinel, Iterator>>
  using encoder = decltype(CodecTraits::make_encoder(std::declval<Iterator>(),
                                                     std::declval<Sentinel>()));

  // TODO SFINAE + detected + codec_traits concept
  template <typename Iterator,
            typename Sentinel = Iterator,
            typename = meta::concepts::iterator::Sentinel<Sentinel, Iterator>>
  using decoder = decltype(CodecTraits::make_decoder(std::declval<Iterator>(),
                                                     std::declval<Sentinel>()));

  using default_encoded_output = typename CodecTraits::default_encoded_output;
  using default_decoded_output = typename CodecTraits::default_decoded_output;

  template <typename Iterator, typename Sentinel>
  static auto make_encoder(
      meta::concepts::iterator::Iterator<Iterator> begin,
      meta::concepts::iterator::Sentinel<Sentinel, Iterator> end)
  {
    return CodecTraits::make_encoder(std::move(begin), std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(
      meta::concepts::iterator::Iterator<Iterator> begin,
      meta::concepts::iterator::Sentinel<Sentinel, Iterator> end)
  {
    return CodecTraits::make_decoder(std::move(begin), std::move(end));
  }

  template <typename T = default_encoded_output,
            typename Iterator = void,
            typename Sentinel = void,
            typename Encoder = adapters::concepts::TransformedInputAdapter<
                encoder<Iterator, Sentinel>>>
  static concepts::CodecOutput<T, Encoder> encode(
      meta::concepts::iterator::Iterator<Iterator> it,
      meta::concepts::iterator::Sentinel<Sentinel, Iterator> sent)
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
  static concepts::CodecOutput<T, Encoder> encode(Iterable const& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::encode<T>(begin(it), end(it));
  }

  template <typename T = default_decoded_output,
            typename Iterator = void,
            typename Sentinel = void,
            typename Decoder = adapters::concepts::TransformedInputAdapter<
                decoder<Iterator, Sentinel>>>
  static concepts::CodecOutput<T, Decoder> decode(
      meta::concepts::iterator::Iterator<Iterator> it,
      meta::concepts::iterator::Sentinel<Sentinel, Iterator> sent)
  {
    using std::begin;
    using std::end;

    auto dec = CodecTraits::make_decoder(it, sent);
    return output_traits<T>::create(dec);
  }

  template <typename T = default_decoded_output,
            typename U = void,
            typename Iterable = meta::concepts::iterator::Iterable<U>,
            typename Iterator = meta::result_of_begin<Iterable const&>,
            typename Sentinel = meta::result_of_end<Iterable const&>,
            typename Decoder = adapters::concepts::TransformedInputAdapter<
                decoder<Iterator, Sentinel>>>
  static concepts::CodecOutput<T, Decoder> decode(Iterable const& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::decode<T>(begin(it), end(it));
  }
};
}
}
}
