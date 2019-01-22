#pragma once

#include <utility>

#include <mgs/codecs/output_traits.hpp>
#include <mgs/concepts/codec_output.hpp>
#include <mgs/concepts/codec_traits.hpp>
#include <mgs/concepts/iterable.hpp>
#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
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
  template <typename Iterator, typename Sentinel = Iterator>
  using encoder = decltype(CodecTraits::make_encoder(std::declval<Iterator>(),
                                                     std::declval<Sentinel>()));

  template <typename Iterator, typename Sentinel = Iterator>
  using decoder = decltype(CodecTraits::make_decoder(std::declval<Iterator>(),
                                                     std::declval<Sentinel>()));

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
            typename Iterable = mgs::Iterable<U>,
            typename Iterator = meta::result_of_begin<Iterable const&>,
            typename Sentinel = meta::result_of_end<Iterable const&>,
            typename Encoder =
                mgs::TransformedInputRange<encoder<Iterator, Sentinel>>>
  static mgs::CodecOutput<T, Encoder> encode(U const& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::encode<T>(begin(it), end(it));
  }

  template <typename T = default_encoded_output,
            typename U,
            typename Iterable = mgs::Iterable<U>,
            typename Iterator = meta::result_of_begin<Iterable&>,
            typename Sentinel = meta::result_of_end<Iterable&>,
            typename Encoder =
                mgs::TransformedInputRange<encoder<Iterator, Sentinel>>>
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
            typename Iterable = mgs::Iterable<U>,
            typename Iterator = meta::result_of_begin<Iterable const&>,
            typename Sentinel = meta::result_of_end<Iterable const&>,
            typename Decoder =
                mgs::TransformedInputRange<decoder<Iterator, Sentinel>>>
  static mgs::CodecOutput<T, Decoder> decode(U const& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::decode<T>(begin(it), end(it));
  }

  template <typename T = default_decoded_output,
            typename U,
            typename Iterable = mgs::Iterable<U>,
            typename Iterator = meta::result_of_begin<Iterable&>,
            typename Sentinel = meta::result_of_end<Iterable&>,
            typename Decoder =
                mgs::TransformedInputRange<decoder<Iterator, Sentinel>>>
  static mgs::CodecOutput<T, Decoder> decode(U& it)
  {
    using std::begin;
    using std::end;

    return basic_codec::decode<T>(begin(it), end(it));
  }

  template <typename T = default_encoded_output,
            typename CharT,
            std::size_t N,
            typename = std::enable_if_t<
                std::is_same<char, std::remove_const_t<CharT>>::value>>
  static mgs::CodecOutput<T, encoder<CharT*>> encode(CharT (&tab)[N])
  {
    auto const begin_it = std::begin(tab);
    auto const end_it = std::find(begin_it, std::end(tab), '\0');
    return encode<T>(begin_it, end_it);
  }

  template <typename T = default_decoded_output,
            typename CharT,
            std::size_t N,
            typename = std::enable_if_t<
                std::is_same<char, std::remove_const_t<CharT>>::value>>
  static mgs::CodecOutput<T, decoder<CharT*>> decode(CharT (&tab)[N])
  {
    auto const begin_it = std::begin(tab);
    auto const end_it = std::find(begin_it, std::end(tab), '\0');
    return decode<T>(begin_it, end_it);
  }
};
}
}
}
