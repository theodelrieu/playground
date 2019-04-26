#pragma once

#include <cstring>
#include <utility>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/concepts/codec_traits.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/concepts/detected/static_member_functions/make_decoder.hpp>
#include <mgs/concepts/detected/static_member_functions/make_encoder.hpp>
#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/sentinel_t.hpp>
#include <mgs/meta/static_asserts.hpp>

namespace mgs
{
namespace detail
{
// FIXME put detected::make_encoder in codecs_base/detail
// put Codec concepts in codecs_base/concepts
template <typename CodecTraits, typename T, typename U>
using encoder = decltype(CodecTraits::make_encoder(std::declval<T>(),
                                                   std::declval<U>()));

template <typename CodecTraits, typename T, typename U>
using decoder = decltype(CodecTraits::make_decoder(std::declval<T>(),
                                                   std::declval<U>()));
}

namespace codecs
{
inline namespace v1
{
template <typename CodecTraits>
class basic_codec
{
  static_assert(is_codec_traits<CodecTraits>::value, "");

public:
  template <typename Iterator, typename Sentinel = Iterator>
  using encoder = meta::detected_t<detail::encoder, CodecTraits, Iterator, Sentinel>;

  template <typename Iterator, typename Sentinel = Iterator>
  using decoder = meta::detected_t<detail::decoder, CodecTraits, Iterator, Sentinel>;

  using default_encoded_output = typename CodecTraits::default_encoded_output;
  using default_decoded_output = typename CodecTraits::default_decoded_output;

  template <typename Iterator, typename Sentinel>
  static mgs::TransformedInputRange<encoder<Iterator, Sentinel>> make_encoder(
      Iterator begin, Sentinel end)
  {
    return CodecTraits::make_encoder(std::move(begin), std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static mgs::TransformedInputRange<decoder<Iterator, Sentinel>> make_decoder(
      Iterator begin, Sentinel end)
  {
    return CodecTraits::make_decoder(std::move(begin), std::move(end));
  }

  template <typename T = default_encoded_output,
            typename Iterator,
            typename Sentinel,
            typename Encoder =
                mgs::TransformedInputRange<encoder<Iterator, Sentinel>>>
  static mgs::CodecOutput<T, Encoder> encode(Iterator it, Sentinel sent)
  {
    auto enc = CodecTraits::make_encoder(it, sent);
    return output_traits<T>::create(enc);
  }

  template <typename T = default_encoded_output,
            typename U,
            typename Range = mgs::meta::Range<U>,
            typename Encoder = mgs::TransformedInputRange<
                encoder<meta::iterator_t<Range const>, meta::sentinel_t<Range const>>>,
            typename = std::enable_if_t<
                !meta::is_convertible_to<U, char const*>::value>>
  static mgs::CodecOutput<T, Encoder> encode(U const& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::encode<T>(begin(it), end(it));
  }

  template <typename T = default_encoded_output,
            typename U,
            typename Range = mgs::meta::Range<U>,
            typename Encoder = mgs::TransformedInputRange<
                encoder<meta::iterator_t<Range>, meta::sentinel_t<Range>>>,
            typename = std::enable_if_t<
                !meta::is_convertible_to<U, char const*>::value>>
  static mgs::CodecOutput<T, Encoder> encode(U& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::encode<T>(begin(it), end(it));
  }

  template <typename T = default_decoded_output,
            typename Iterator,
            typename Sentinel,
            typename Decoder =
                mgs::TransformedInputRange<decoder<Iterator, Sentinel>>>
  static mgs::CodecOutput<T, Decoder> decode(Iterator it, Sentinel sent)
  {
    auto dec = CodecTraits::make_decoder(std::move(it), std::move(sent));
    return output_traits<T>::create(dec);
  }

  template <typename T = default_decoded_output,
            typename U,
            typename Range = mgs::meta::Range<U>,
            typename Decoder = mgs::TransformedInputRange<
                decoder<meta::iterator_t<Range const>, meta::sentinel_t<Range const>>>,
            typename = std::enable_if_t<
                !meta::is_convertible_to<U, char const*>::value>>
  static mgs::CodecOutput<T, Decoder> decode(U const& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::decode<T>(begin(it), end(it));
  }

  template <typename T = default_decoded_output,
            typename U,
            typename Range = mgs::meta::Range<U>,
            typename Decoder = mgs::TransformedInputRange<
                decoder<meta::iterator_t<Range>, meta::sentinel_t<Range>>>,
            typename = std::enable_if_t<
                !meta::is_convertible_to<U, char const*>::value>>
  static mgs::CodecOutput<T, Decoder> decode(U& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::decode<T>(begin(it), end(it));
  }

  template <typename T = default_decoded_output>
  static mgs::CodecOutput<T, decoder<char const*>> decode(char const* s)
  {
    return decode<T>(s, s + std::strlen(s));
  }

  template <typename T = default_encoded_output>
  static mgs::CodecOutput<T, encoder<char const*>> encode(char const* s)
  {
    return encode<T>(s, s + std::strlen(s));
  }
};
}
}
}
